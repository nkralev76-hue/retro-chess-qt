#!/usr/bin/env bash
# Assembles a portable Windows bundle from build/RetroChess.exe.
#
# windeployqt copies the Qt libraries and plugins, but neither the MinGW
# runtime nor the third party libraries Qt itself links against (icu,
# freetype, harfbuzz, libpng, zlib, ...). Without those the executable only
# starts on a machine that happens to have MSYS2 on PATH, so resolve them
# from the active MSYS2 prefix and copy them in as well.
#
# Usage: scripts/deploy-windows.sh [build-dir] [output-dir]
# Run from an MSYS2 shell with the matching environment active.
set -euo pipefail

src="${1:-build}"
out="${2:-RetroChess-win}"

# The prefix has to be derived from the Qt tools actually in use: MSYS_PREFIX is
# not always exported (running the script through plain `sh` drops it), and a
# wrong guess silently produces a bundle that resolves nothing.
qt_bin="$(command -v windeployqt6 || true)"
if [ -n "$qt_bin" ]; then
	prefix="$(dirname "$(dirname "$(readlink -f "$qt_bin")")")"
else
	prefix="${MSYS_PREFIX:-/mingw64}"
fi
[ -d "$prefix/bin" ] || {
	echo "error: cannot determine the MSYS2 prefix (got '$prefix')" >&2
	exit 1
}

if [ ! -f "$src/RetroChess.exe" ]; then
	echo "error: $src/RetroChess.exe not found - build the project first" >&2
	exit 1
fi

rm -rf "$out"
mkdir -p "$out"

windeployqt6 --release --no-translations \
	--dir "$(pwd -W)/$out" "$(pwd -W)/$src/RetroChess.exe" >/dev/null
cp "$src/RetroChess.exe" "$out/"

# Every library the bundle actually needs that is not already in it.
bundled_deps() {
	local f
	for f in "$out"/*.dll "$out"/*.exe; do
		[ -e "$f" ] || continue
		ldd "$f" 2>/dev/null |
			sed -n 's/.*=>[[:space:]]*//p' |
			awk '{print $1}' |
			grep "^$prefix/bin/" || true
	done
}

# Repeat until the set stops growing: a freshly copied library can pull in
# further dependencies of its own.
for _ in 1 2 3 4; do
	copied=0
	while read -r dep; do
		[ -n "$dep" ] || continue
		[ -e "$out/$(basename "$dep")" ] && continue
		cp "$dep" "$out/"
		copied=1
	done <<< "$(bundled_deps | sort -u)"
	[ "$copied" -eq 1 ] || break
done

# Fail loudly rather than shipping a bundle that only starts on a machine with
# MSYS2 installed - that is exactly the bug this script exists to prevent.
left="$(bundled_deps | sort -u)"
if [ -n "$left" ]; then
	echo "error: unresolved dependencies in $out:" >&2
	echo "$left" >&2
	exit 1
fi

echo "Bundle ready: $out ($(find "$out" -type f | wc -l) files, prefix $prefix)"