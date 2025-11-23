#!/usr/bin/env bash
set -euo pipefail

# normalize_test_paths.sh
# Scan files under a directory (default: test-code) for quoted paths that start with
# "test/" (e.g. "test/test-files/...") and remove the leading "test/" portion.
# By default the script performs a dry-run and prints matches. Use --apply to modify files in-place.

DIR="test-code"
APPLY=0

usage() {
    cat <<EOF
Usage: $0 [--dir DIR] [--apply] [--help]

Scans files under DIR (default: test-code) for quoted paths that begin with "test/"
and removes the leading "test/" from those quoted paths. Runs as a dry-run unless
--apply is provided. When --apply is used a .bak backup file is created for each
modified file.
EOF
}

while [[ $# -gt 0 ]]; do
    case "$1" in
        --dir) DIR="$2"; shift 2;;
        --apply) APPLY=1; shift;;
        -h|--help) usage; exit 0;;
        *) echo "Unknown arg: $1"; usage; exit 2;;
    esac
done

if [ ! -d "$DIR" ]; then
    echo "Directory '$DIR' not found." >&2
    exit 2
fi

echo "Scanning directory: $DIR"
if [ $APPLY -eq 0 ]; then
    echo "DRY RUN (no files will be modified). Use --apply to make changes."
else
    echo "APPLY MODE: files will be edited in-place and a .bak backup created." 
fi

matches=0
changes=0

# find text files under DIR; grep -I will ignore binaries when searching
while IFS= read -r file; do
    # skip hidden VCS directories
    case "$file" in
        */.git/*|*/.svn/*|*/.hg/*) continue;;
    esac

    if grep -I -n -E "['\"]test/[^'\"[:space:]]+" "$file" >/dev/null 2>&1; then
        matches=$((matches+1))
        echo "\nMATCH: $file"
        grep -n -E "['\"]test/[^'\"[:space:]]+" "$file" || true

        if [ $APPLY -eq 1 ]; then
            # Use Perl with lookbehind/lookahead to only remove the leading test/ when
            # it's immediately after a quote and before a non-quote/non-space character.
            perl -0777 -pe "s/(?<=['\"])test\\/(?=[^'\"\\s])//g" -i.bak "$file"
            echo "  -> updated (backup: $file.bak)"
            changes=$((changes+1))
        fi
    fi
done < <(find "$DIR" -type f -print)

echo "\nSummary: $matches files contain matches; $changes files changed."

if [ $APPLY -eq 1 ] && [ $changes -gt 0 ]; then
    echo "You can inspect backups (*.bak) or run 'git diff' to review changes."
fi

exit 0
