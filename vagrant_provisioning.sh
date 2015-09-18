echo "Starting provisioning..."

# Set locale to avoid warnings
export LANGUAGE=en_US.UTF-8
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
sudo locale-gen en_US.UTF-8
sudo dpkg-reconfigure locales

if [[ $(ldconfig -p | grep libhidapi-libusb) ]]; then
    echo "Found library \"libhidapi\"."
else
    echo "Installing libhidapi..."
    sudo apt-get -qq -y --no-install-recommends install build-essential
    sudo apt-get -qq -y --no-install-recommends install pkg-config
    sudo apt-get -qq -y --no-install-recommends install libudev-dev
    sudo apt-get -qq -y --no-install-recommends install libusb-1.0-0-dev
    sudo apt-get -qq -y --no-install-recommends install autotools-dev
    sudo apt-get -qq -y --no-install-recommends install autoconf
    sudo apt-get -qq -y --no-install-recommends install automake
    sudo apt-get -qq -y --no-install-recommends install libtool
	sudo apt-get -qq -y --no-install-recommends install git
	sudo ldconfig
	git clone https://github.com/signal11/hidapi.git
	cd hidapi
	sudo ./bootstrap
	sudo ./configure
	sudo make
	sudo make install
	sudo ldconfig
	cd /home/vagrant/
	echo "Finished."
fi

if [[ $(ldconfig -p | grep libcmocka) ]]; then
    echo "Found library \"cmocka\"."
else
    echo "Installing cmocka unit test framework..."
	sudo apt-get -qq -y --no-install-recommends install cmake
	sudo apt-get -qq -y --no-install-recommends install git
	git clone --branch cmocka-1.0.1 https://git.cryptomilk.org/projects/cmocka.git
	cd cmocka
	mkdir build
	cd build
	cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Debug ..
	make
	sudo make install
	sudo ldconfig
	cd /home/vagrant/
	echo "Finished."
fi

if [[ $( ls /usr/local/lib/ | grep libpiclib.a) ]]; then
    echo "Found library \"piclib\"."
else
    echo "Installing piclib..."
	sudo apt-get -qq -y --no-install-recommends install git
	git clone https://github.com/sebastianhaas/piclib.git
	cd piclib
	make
	sudo make install
	sudo ldconfig
	cd /home/vagrant/
	echo "Finished."
fi

# echo "Running unit tests..."
# cd /vagrant
# make clean
# make
# ./piclibtest
# make clean
# cd ~
# echo "Finished unit test run."

echo "Finished provisioning. Your machine is ready to use."