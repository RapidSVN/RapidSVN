TESTING NOTES

INTRODUCTION

Testing is done with CppUnit (http://cppunit.sourceforge.net/).  Download 
and untar the source.  Rename the directory to 'cppunit' and build. 

* Run the ./make_repos.sh (or make_repos.bat) file to create the test 
repository used by the tests.  This will probably be replaced by a Python 
script in the future.  There is a REPOS_PATH variable in it that directs it
the absolute path of the test directory.  Change it to your directory.

* Copy and rename the "test_config.ah" to "test_config.h" and add in a valid 
directory on your computer.  Open it and set the "REPOS_PATH" to the same path
set in the make_respos file.

* Build the tests with the file designed for your environment. Right now the 
  the tests get compiled but not linked with Unix. Feel free to rectify this.

TIPS

* Keep in mind that if the format of the filesystem is changed then you 
will need to get a new version of svn in order to make it work correctly.

* Use only the newest versions of RPMs if you are on Linux because the API
changes so often that you might find yourself trying to debug issues that 
no longer exist.
