Markdown preview
================

This Vagrant box will install [grip](https://github.com/joeyespo/grip) as local
Markdown renderer, thus enabling preview of changes to e.g. `README.md` without
pushing to GitHub.


Usage
-----

    host> vagrant up
    host> vagrant ssh
    box> ./preview.sh
    ^C
    box> exit
    host> vagrant destroy -f

Executing `preview.sh` inside the Vagrant box will start `grip` at the root
directory of the violetland repository. Since the Vagrant box is configured to
forward port 6419 (default `grip` port) you should be able to accessing the
`README.md` Markdown preview at

    http://ip-of-host:6419/README.md

The `preview.sh` script will also list all paths of Markdown files inside the
repository.

