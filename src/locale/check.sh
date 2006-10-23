#!/bin/bash
#
# This script which of the translations contain "fuzzy" strings
# that have to be checked and corrected by the translators
echo "These files have to be checked:"
find -name rapidsvn.po -exec grep -l fuzzy {} \;

