# A bash script to synchronize the git repository with bitbucket.
#
# Usage:
#   ./syncBitbucket.sh pull
#   ./syncBitbucket.sh push
#
# Author: Ying Xiong.
# Created: Dec 03, 2013.

HelpMessage="Usage: ./syncBitbucket.sh pull/push"

if [ $# != 1 ]; then
    echo $HelpMessage
    exit 1
fi

if [ $1 == "pull" ]; then
    branch=$(git branch | grep '*' | sed s/'* '//)
    echo "==== Currently in '$branch', begin to pull from bitbucket. ===="
    set -x -e
    git checkout master
    git fetch bitbucket
    git merge bitbucket/master
    git checkout $branch
    git merge master
    set +x +e
    echo "==== Done. ===="
elif [ $1 == "push" ]; then
    branch=$(git branch | grep '*' | sed s/'* '//)
    echo "==== Currently in '$branch', begin to push to bitbucket. ===="
    set -x -e
    git checkout master
    git merge $branch
    git push bitbucket --all
    git push bitbucket --tags
    git checkout $branch
    set +x +e
    echo "==== Done. ===="
else
    echo $HelpMessage
    exit 1
fi
