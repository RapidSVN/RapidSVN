import unittest
import check_license
import re

class TestCheckLicense(unittest.TestCase):
    def test_get_converter_for_cpp(self):
        converter = check_license.get_converter('foo.cpp')
        self.assertIsNotNone(converter)

    def test_get_converter_for_embedded_png(self):
        self.assertIsNone(check_license.get_converter('foo/bar.png.h'))

    def test_shell_regex(self):
        r = check_license.OLD_LICENSE_SHELL
        self.assertIsNotNone(r)
        with open('testfiles/license_with_shell', 'r') as file:
            t = file.read()
            self.assertIsNotNone(re.search(r, t))
        
    def test_txt_regex(self):
        r = check_license.OLD_LICENSE_TXT
        self.assertIsNotNone(r)
        with open('testfiles/license_with_txt', 'r') as file:
            t = file.read()
            self.assertIsNotNone(re.search(r, t))

    def test_cpp_regex(self):
        r = check_license.OLD_LICENSE_CPP
        self.assertIsNotNone(r)
        with open('testfiles/license_with_cpp', 'r') as file:
            t = file.read()
            self.assertIsNotNone(re.search(r, t))
