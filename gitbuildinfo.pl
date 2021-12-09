#!/usr/bin/perl

$commit_number=`git describe --long`;
$commit_date=`git show -s --format="%cd"`;
$commit_rev=`git show -s --format="%H"`;

chop($commit_number);
chop($commit_date);
chop($commit_rev);

print "$commit_date - $commit_number\n";

