# $Id$

# quick utility for switching all filenames and function names therein from 'profile' to 'profile2'.
# note no shebang line so this can't be executed by a server: invoke this with perl itself.

use warnings;
$\ = "\n";

# config
$root = $from = $to = 'profile';

########

if ($ARGV[0] eq '-r') {
  $from .= '2'; 
}
else {
  $to   .= '2'; 
}

print "Root is $root, converting from $from to $to.";

# rename files

@files = glob("$from*");
foreach $oldfile (@files) {
  ($newfile = $oldfile) =~ s[^$from][$to];
  print " Renaming $oldfile to $newfile.";
  rename $oldfile, $newfile;
}

# change all function and constant names

# see http://perldoc.perl.org/perlfaq5.html#How-can-I-use-Perl%27s--i-option-from-within-a-program?
{
  local $^I   = '';               # in-place editing, like the -i switch.
  local @ARGV = glob("$from*");   # get the files back as they have changed name
  local $\    = '';               # reset print output
  while (<>) {
    # this line is handy for testing  
    #s/function/pony/g;
    
    s[\b$from(?=_)][$to]gi;
  
    print;
  }
}