#!/bin/sh

set -eu

repository_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
credential_path="SORIExample/SORICredentials.swift"
template_path="${credential_path}.example"
test_suffix="$(date +%s)_$$"
test_application_id="SORI_STAGE_TEST_APPLICATION_ID_${test_suffix}"
test_secret_key="SORI_STAGE_TEST_SECRET_KEY_${test_suffix}"
scratch_directory=$(mktemp -d "${TMPDIR:-/tmp}/sori-credential-validation.XXXXXX")
test_repository="${scratch_directory}/repository"

cleanup() {
    rm -rf "$scratch_directory"
}
trap cleanup EXIT HUP INT TERM

count_occurrences() {
    value=$1
    file=$2
    count=$(grep -oF "$value" "$file" | wc -l | tr -d ' ')
    [ "$count" -eq 1 ] || {
        echo "Expected $value exactly once in $file; found $count." >&2
        exit 1
    }
}

count_occurrences "YOUR_SORI_APPLICATION_ID" "${repository_root}/${template_path}"
count_occurrences "YOUR_SORI_SECRET_KEY" "${repository_root}/${template_path}"

mkdir -p "$test_repository"
git -C "$repository_root" ls-files --cached --others --exclude-standard |
while IFS= read -r path; do
    mkdir -p "${test_repository}/$(dirname -- "$path")"
    cp "${repository_root}/${path}" "${test_repository}/${path}"
done

cp "${test_repository}/${template_path}" "${test_repository}/${credential_path}"
sed -i.bak \
    -e "s/YOUR_SORI_APPLICATION_ID/${test_application_id}/" \
    -e "s/YOUR_SORI_SECRET_KEY/${test_secret_key}/" \
    "${test_repository}/${credential_path}"
rm "${test_repository}/${credential_path}.bak"

git -C "$test_repository" init -q
ignore_output=$(git -C "$test_repository" check-ignore -v "$credential_path")
tab=$(printf '\t')
ignore_source=${ignore_output%%"$tab"*}
ignored_path=${ignore_output#*"$tab"}
case "$ignore_source" in
    .gitignore:*:/SORIExample/SORICredentials.swift) ;;
    *)
        echo "Unexpected ignore rule for $credential_path: $ignore_output" >&2
        exit 1
        ;;
esac
[ "$ignored_path" = "$credential_path" ] || {
    echo "Unexpected ignored path for $credential_path: $ignore_output" >&2
    exit 1
}

git -C "$test_repository" add .

if git -C "$test_repository" diff --cached --name-only | grep -Fxq "$credential_path"; then
    echo "$credential_path was staged unexpectedly." >&2
    exit 1
fi

for expected_path in \
    .gitignore \
    README.md \
    SORIExample.xcodeproj/project.pbxproj \
    SORIExample/ContentView.swift \
    "$template_path" \
    scripts/validate-credential-seam.sh
do
    git -C "$test_repository" diff --cached --name-only | grep -Fxq "$expected_path" || {
        echo "Expected tracked project file was not staged: $expected_path" >&2
        exit 1
    }
done

if git -C "$test_repository" grep --cached -F "$test_application_id" >/dev/null 2>&1; then
    echo "Test application ID was found in staged content." >&2
    exit 1
fi

if git -C "$test_repository" grep --cached -F "$test_secret_key" >/dev/null 2>&1; then
    echo "Test Secret Key was found in staged content." >&2
    exit 1
fi

echo "Credential seam validation passed."
