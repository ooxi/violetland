/**
 * Generates build environments for various Debian derivates. Each build
 * environment consits of the following machines:
 *
 *
 *  1) create-orig-tarball
 *
 *     Creates a `violetland_<version>.orig.tar.gz' file containing the
 *     violetland sources. GIT submodules will be resolved and SCM metadata
 *     will be removed.
 *
 *
 *  2) package-for-debian
 *
 *     Provides all necessary build dependencies and debian packaging helpers.
 *     Creates the `.deb' and associated files.
 *
 *  3) test-package
 *
 *     Installs the fresh package to ensure all dependencies are correctly
 *     resolved.
 */
var fs = require("fs");





/* Currently build version
 */
var violetland_version = fs.readFileSync("../../VERSION", "utf8").trim();


/* Base build dependencies (Debian package names) shared by all flavours
 */
var base_build_dependencies = [
	"build-essential",
	"cmake",
	"gettext",
	"libsdl-ttf2.0-dev",
	"libsdl-mixer1.2-dev",
	"libsdl-image1.2-dev",
	"libboost-all-dev"
];



/* Supported flavours
 */
var debian_flavours = [

	// Ubuntu 15.04
	{	name:		"ubuntu-15.04-amd64",
		image:		"ubuntu/vivid64",
		dependencies:	base_build_dependencies
	},
	
	{	name:		"ubuntu-15.04-x86",
		image:		"ubuntu/vivid32",
		dependencies:	base_build_dependencies
	},

	// Ubuntu 14.04
	{	name:		"ubuntu-14.04-amd64",
		image:		"ubuntu/trusty64",
		dependencies:	base_build_dependencies
	},
	
	{	name:		"ubuntu-14.04-x86",
		image:		"ubuntu/trusty32",
		dependencies:	base_build_dependencies
	},

	// Ubuntu 12.04
	{	name:		"ubuntu-12.04-amd64",
		image:		"ubuntu/precise64",
		dependencies:	base_build_dependencies
	},
	
	{	name:		"ubuntu-12.04-x86",
		image:		"ubuntu/precise32",
		dependencies:	base_build_dependencies
	}
];





/**
 * Reads one file, replaces all occurences of keywords with values and writes
 * the result into another file (assuming UTF-8)
 *
 * @param string input_file Path to input file
 * @param string output_file Path to output file
 * @param {key → value} replacements Replacement map
 */
var apply_template = function(input_file, output_file, replacements) {
	var input = fs.readFileSync(input_file, {encoding: "utf8"});
	
	for (var key in replacements) {
		var value = replacements[key];
		input = input.split("%" + key + "%").join(value);
	}
	
	fs.writeFileSync(output_file, input, {encoding: "utf8"});
};





/* Create Vagrantfiles for every flavour
 */
var flavours_directory = "flavours";
fs.mkdirSync(flavours_directory);

var bash_flavours_script = ""+
	"#!/bin/bash\n"+
	"\n";



for (var i = 0; i < debian_flavours.length; ++i) {
	var debian_flavour = debian_flavours[i];
	
	
	/* Setup template replacements
	 */
	var replacements = {
		"FLAVOUR_IMAGE":	debian_flavour.image,
		"SOURCE_DIRECTORY":	"../../../../..",
		"FLAVOUR_DIRECTORY":	"..",
		"VIOLETLAND_VERSION":	violetland_version,

		// HOTFIX
		"DATE_RFC_1123":	new Date().toUTCString().replace("GMT", "+0200"),
		"CONTROL_DEPENDENCIES":	debian_flavour.dependencies.join(",\n               "),
		"APT_GET_DEPENDENCIES":	debian_flavour.dependencies.join(" ")
	};
	

	/* Create image directory
	 */
	var flavour_directory = flavours_directory + "/" + debian_flavour.name;
	fs.mkdirSync(flavour_directory);
	
	
	/* Machine `create-orig-tarball'
	 */
	var create_orig_tarball_directory = flavour_directory + "/create-orig-tarball";
	fs.mkdirSync(create_orig_tarball_directory);
	
	apply_template(
		"create-orig-tarball.Vagrantfile", 
		create_orig_tarball_directory + "/Vagrantfile",
		replacements
	);
	
	
	/* Machine `package-for-debian'
	 */
	var package_for_debian_directory = flavour_directory + "/package-for-debian";
	fs.mkdirSync(package_for_debian_directory);
	
	
	fs.mkdirSync(package_for_debian_directory + "/debian");
	fs.mkdirSync(package_for_debian_directory + "/debian/source");
	
	apply_template(
		"debian/source/format",
		package_for_debian_directory + "/debian/source/format",
		replacements
	);
	
	apply_template(
		"debian/changelog",
		package_for_debian_directory + "/debian/changelog",
		replacements
	);
	
	apply_template(
		"debian/compat",
		package_for_debian_directory + "/debian/compat",
		replacements
	);
	
	apply_template(
		"debian/control",
		package_for_debian_directory + "/debian/control",
		replacements
	);
	
	apply_template(
		"debian/copyright",
		package_for_debian_directory + "/debian/copyright",
		replacements
	);
	
	apply_template(
		"debian/rules",
		package_for_debian_directory + "/debian/rules",
		replacements
	);
	
	apply_template(
		"package-for-debian.Vagrantfile",
		package_for_debian_directory + "/Vagrantfile",
		replacements
	);


	/* Machine `test-package'
	 */
	var test_package_directory = flavour_directory + "/test-package";
	fs.mkdirSync(test_package_directory);
	
	apply_template(
		"test-package.Vagrantfile", 
		test_package_directory + "/Vagrantfile",
		replacements
	);


	/* A simple shell script spinning up and shutting down all machines
	 */
	var bash_flavour_script = ""+
		"#!/bin/bash\n"+
		"\n"+
		"bash -c \"cd create-orig-tarball; vagrant up; vagrant destroy -f\"\n"+
		"bash -c \"cd package-for-debian; vagrant up; vagrant destroy -f\"\n"+
		"bash -c \"cd test-package; vagrant up; vagrant destroy -f\"\n";

	fs.writeFileSync(flavour_directory +"/build.sh", bash_flavour_script, {
		encoding:	'utf8',
		mode:		0766
	});

	bash_flavours_script += "bash -c \"cd '"+ debian_flavour.name +"'; ./build.sh\"\n";
}



/* Shell script building all flavours
 */
fs.writeFileSync(flavours_directory +"/build.sh", bash_flavours_script, {
	encoding:	'utf8',
	mode:		0766
});

