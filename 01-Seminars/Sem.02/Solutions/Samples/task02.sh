find / -type d -name "dir" 2>/dev/null
# / - root directory
# 2>/dev/null - all errors are "ignored"
# FYI: 2> redirects the standard error stream to /dev/null, which effectively discards any error messages generated during the search.