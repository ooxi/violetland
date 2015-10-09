# -*- mode: ruby -*-
# vi: set ft=ruby :

VAGRANTFILE_API_VERSION = "2"

Vagrant.configure(VAGRANTFILE_API_VERSION) do |config|
	config.vm.box = "%FLAVOUR_IMAGE%"

	config.vm.provision "file", source: "%SOURCE_DIRECTORY%", destination: "/home/vagrant/violetland"
	config.vm.synced_folder "%FLAVOUR_DIRECTORY%", "/home/vagrant/flavour"

	config.vm.provision "shell", inline: <<-SHELL
	
		# Install necessary utilities
		sudo apt-get update
		sudo apt-get install -y git gzip tar

		# Download submodules
		cd violetland
		git submodule init
		git submodule update
		cd ..

		# Remove build system and SCM files
		rm -rf violetland/.git
		rm -rf violetland/lib/CMake-Gettext/.git
		rm -rf violetland/lib/fcitx/.git
		rm -rf violetland/vagrant

		# Package and export upstream tarball
		tar -czf "violetland_%VIOLETLAND_VERSION%.orig.tar.gz" violetland
		mv "violetland_%VIOLETLAND_VERSION%.orig.tar.gz" /home/vagrant/flavour
	SHELL

end

