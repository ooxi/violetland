How to add a translation
========================

In order to enable as many people as possible this the joy of playing Violetland
we are aming at providing localized versions in all major languages. Since
creating a complete localization from scratch is a big effort, the preferred way
is to contribute to the crowdsourced translations on
[GetLocalized](http://www.getlocalization.com/violetland/).

After a localization is finished, a new locale has to be added to `LANGUAGES`,
for example `fr` for "french".

Now download the appropriate po files from
[GetLocalized](http://www.getlocalization.com/violetland/files/) and apply the
necessary adaptions. You may want to use a po enabled editor like
[poedit](http://www.poedit.net/) or [vim](http://www.vim.org/).

Don't forget to configure the package name and version. And also the charset of
the catalog (I use utf-8 for french)

When you have finished run `compile.sh` and launch violetland to test it!

