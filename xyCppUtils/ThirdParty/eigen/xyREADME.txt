================================================================
Eigen --- a C++ template library for linear algebra: matrices, vectors,
numerical solvers, and related algorithms.
================================================================

Accessed from: http://eigen.tuxfamily.org/
Accessed by: Ying Xiong.
Accessed on: Dec 02, 2013 (v3.2).

================================================================
Source control.
================================================================
The source code is hosted on bitbucket as a git repository.

Access the repository:
  $ git clone git@bitbucket.org:ylxiong/thirdparty-eigen.git eigen
  $ cd eigen
  $ git remote rename origin bitbucket
  $ git checkout -b child-branch

Update with bitbucket:
  $ ./syncBitbucket.sh pull
  $ ./syncBitbucket.sh push
or equivalently:
  $ git checkout master
  $ git fetch --all
  $ git merge bitbucket/master
  $ git merge child-branch
  $ git push bitbucket --all
  $ git push bitbucket --tags
  $ git checkout child-branch

Update with 'upstream' (author's distribution):
  $ ./syncBitbucket.sh pull
  $ git checkout upstream
  $ git merge bitbucket/upstream
  # Update with author's release and commit to 'upstream' branch.
  $ git checkout child-branch
  $ git merge master
  $ ./syncBitbucket.sh push

Note: if ssh with bitbuket has not been setup at properly at local machine, one
can use https://bitbucket.org/ylxiong/thirdparty-eigen as remote url, but this
requires authentication every time connecting to the host.

================================================================
Compile and run.
================================================================
Notes for compiling and running the code.
