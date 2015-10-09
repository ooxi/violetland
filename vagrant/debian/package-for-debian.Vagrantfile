# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
	config.vm.box = "%FLAVOUR_IMAGE%"


	# Access to original sources tarball and export of build packages
	config.vm.synced_folder "%FLAVOUR_DIRECTORY%", "/home/vagrant/flavour"


	# Install necessary dependencies in order to create debian packages
	config.vm.provision "shell", inline: <<-SHELL
		sudo apt-get update


		# Install build dependencies
		sudo apt-get install -y %APT_GET_DEPENDENCIES%

		# Install Debian package utilities
		sudo apt-get install -y debhelper devscripts gzip tar


		# Unpack upstream sources and add Debian specific files
		cp "flavour/violetland_%VIOLETLAND_VERSION%.orig.tar.gz" .
		tar -xf "violetland_%VIOLETLAND_VERSION%.orig.tar.gz"
		cp -r "flavour/package-for-debian/debian" violetland
		chown -R vagrant:vagrant violetland


		# Build Debian package
		cd violetland
		debuild -us -uc
		cd ..


		# Publish artifacts
		cp violetland_%VIOLETLAND_VERSION%_*.build flavour
		cp violetland_%VIOLETLAND_VERSION%_*.changes flavour
		cp violetland_%VIOLETLAND_VERSION%.debian.tar.?z flavour
		cp violetland_%VIOLETLAND_VERSION%.dsc flavour
		cp violetland_%VIOLETLAND_VERSION%_*.deb flavour

	SHELL

	
end
