# !/bin/sh
# Unix repository test script

# change this:
REPOS_PATH=file:///home/brent/dev/rapidsvn/tests/svncpp/test_repos
svnadmin create test_repos

svn co $REPOS_PATH my_repos

cp README.txt my_repos
cp *.cpp my_repos
cp *.h my_repos
mkdir my_repos/my_dir
svn add my_repos/README.txt my_repos/main.cpp my_repos/my_dir
svn propset test test_value my_repos/main.cpp
svn propset test2 test_value2 my_repos/main.cpp
svn ci -m "Initial import" my_repos

echo "junk data" > my_repos/README.txt
svn ci -m "changed README.txt" my_repos

echo "more junk data" > my_repos/main.cpp