#!/bin/bash

# Check if two arguments are provided
if [[ ${#} -ne 2 ]]; then
    echo "Error: Invalid number of arguments: ${0}"
    exit 1
fi

package_path=$2

# Check if package directory exists and has required structure
if [[ ! -d "${package_path}" || ! -f "${package_path}/version" || ! -d "${package_path}/tree" ]]; then
    echo "Invalid package structure"
    exit 2
fi

# Check if repository has required permissions
if [[ ! -r "${package_path}" || ! -r "${package_path}/version" || ! -r "${package_path}/tree" ]]; then
    echo "Invalid permissions in ${package_path}"
    exit 3
fi

# Check if repository directory exists and has required structure
if [[ ! -d "${repo_path}" || ! -f "${db_file}" || ! -d "${packages_dir}" ]]; then
    echo "Invalid repository structure"
    exit 4
fi

# Check if repository has required permissions
if [[ ! -r "${repo_path}" || ! -r "${db_file}" || ! -r "${packages_dir}" ]]; then
    echo "Invalid read permissions in ${repo_path}"
    exit 5
fi

if [[ ! -w "${repo_path}" || ! -w "${db_file}" || ! -w "${packages_dir}" ]]; then
    echo "Invalid write permissions in ${repo_path}"
    exit 6
fi

# Read package name and version
package_name=$(basename "${package_path}")
package_version=$(cat "${package_path}/version")

# Archive and compress the tree directory
tar -cf - -C "$package_path" tree | xz -z > "/tmp/${package_name}.tar.xz"

# Calculate the checksum
checksum=$(sha256sum "/tmp/${package_name}.tar.xz" | cut -d ' ' -f 1)

# Update repository
repo_path=$1
db_file="${repo_path}/db"
packages_dir="${repo_path}/packages"

# Check for existing version
if grep -q "${package_name}-${package_version}" "${db_file}"; then
    # Replace existing package
    existing_checksum=$(grep "${package_name}-${package_version}" "$db_file" | cut -d ' ' -f 2)
    mv "/tmp/${package_name}.tar.xz" "${packages_dir}/${existing_checksum}.tar.xz"
else
    # Add new package version
    echo "${package_name}-${package_version} ${checksum}" >> "${db_file}"
    mv "/tmp/${package_name}.tar.xz" "${packages_dir}/${checksum}.tar.xz"

    # Sort db file lexicographically
    sort "${db_file}" -o "${db_file}"
fi

echo "Package ${package_name}-${package_version} added to the repository."