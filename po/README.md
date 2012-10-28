How to add a translation
========================

In order to enable as many people as possible this the joy of playing Violetland
we are aming at providing localized versions in all major languages. Since
creating a complete localization from scratch is a big effort, the preferred way
is to contribute to the crowdsourced translations on
[GetLocalized](http://www.getlocalization.com/violetland/).

After a localization is finished, a new language has to be added to `LINGUAS`,
for example `fr` for "french". A list of supported language codes can be found
in the [gettext manual](http://www.gnu.org/software/gettext/manual/html_node/Usual-Language-Codes.html).

Now download the appropriate po files from
[GetLocalized](http://www.getlocalization.com/violetland/files/) and apply the
necessary adaptions. Most likely you have to rename the file from
`violetland-<language>.pot` to `<language>.po` and specify [plural
forms](http://www.gnu.org/savannah-checkouts/gnu/gettext/manual/html_node/Plural-forms.html).
You may want to use a po enabled editor like [poedit](http://www.poedit.net/) or
[vim](http://www.vim.org/).

Support for the new language will now automatically be generated at build time.
To test the new language, you can modify your `LANG` environment variable before
running `violetland`.

