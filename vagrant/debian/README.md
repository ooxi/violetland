Debian
======

Since all supporte Debian versions (and derivates) behave similar, a common
build environment is provided. The [Vagrantfile](Vagrantfile) in this directory
uses [build.js](build.js) to generate three machines from their respective
templates.

 * The [first machine](create-orig-tarball.Vagrantfile) creates a tarball from
   the current sources
 * The [second machine](package-for-debian.Vagrantfile) builds a debian binary
   package containing all game assets. This is not ideal for a real repository
   but having everything in one `.deb` is more practical for manual
   installation
 * The [third machine](test-package.Vagrantfile) installs the generated package
   and executes the `violetland` binary in order to verify all dependencies are
   resolved correctly

