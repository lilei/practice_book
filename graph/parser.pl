#!/usr/bin/perl
use utf8;
use warnings;
use strict;
use 5.010;

use Graph::Easy::Parser;
foreach (@ARGV) {
  say Graph::Easy::Parser->from_file($_)->as_ascii();
}
