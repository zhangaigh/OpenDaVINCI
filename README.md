# OpenDaVINCI

OpenDaVINCI is a compact middleware OpenDaVINCI written entirely in standard C++.
It runs on a variety of POSIX-compatible OS. And Windows.

You will find its full documentation here: http://opendavinci.readthedocs.org/en/latest/

You will find different projects that are using OpenDaVINCI here: http://opendavinci.cse.chalmers.se/www/index.html

## Using OpenDaVINCI in your Projects

To explore the features of OpenDaVINCI and to use it in your own projects, we provide pre-compiled
packages in .deb and .rpm format for x86, x86_64, and armfh.

### Using our Docker images

We also provide pre-built Docker images for x86_64 and armhf:

* Ubuntu/x86_64: docker pull seresearch/opendavinci-ubuntu-amd64

* Ubuntu/armhf: docker pull seresearch/opendavinci-ubuntu-armhf

### Adding OpenDaVINCI to your Ubuntu Linux distribution

To explore the features of OpenDaVINCI and to use it in your own projects, you can add our .deb repository. We provide
packages for x86, x86_64, and armfh.

1. Add the public key from our repository:

    $ sudo wget -O - -q http://opendavinci.cse.chalmers.se/opendavinci.cse.chalmers.se.gpg.key | sudo apt-key add -

2. Add our repository itself to your sources.list:

    $ sudo echo "deb http://opendavinci.cse.chalmers.se/ubuntu/ trusty main" >> /etc/apt/sources.list

3. Update your package database:

    $ sudo apt-get update

4. Install OpenDaVINCI:

    $ sudo apt-get install opendavinci-lib opendavinci-tools opendavinci-supercomponent


### Adding OpenDaVINCI to your Debian Linux distribution

1. Add the public key from our repository:

    $ sudo wget -O - -q http://opendavinci.cse.chalmers.se/opendavinci.cse.chalmers.se.gpg.key | sudo apt-key add -

2. Add our repository itself to your sources.list:

    $ sudo echo "deb http://opendavinci.cse.chalmers.se/debian/ wheezy main" >> /etc/apt/sources.list

3. Update your package database:

    $ sudo apt-get update

4. Install OpenDaVINCI:

    $ sudo apt-get install opendavinci-lib opendavinci-tools opendavinci-supercomponent


### Adding OpenDaVINCI to your Fedora Linux distribution

1. Add our repository itself to your sources.list:

    $ sudo echo "[myrepo]\r\nname=My Repository\r\nbaseurl=deb http://opendavinci.cse.chalmers.se/rpm-x86_64/repo\r\nenabled=1" >> etc/yum.repos.d/my.repo

2. Update your package database:

   $ sudo yum -y update

3. Install OpenDaVINCI:

   $ sudo yum --nogpgcheck install opendavinci-lib opendavinci-tools opendavinci-supercomponent
   
   
### Adding OpenDaVINCI to your openSuse Linux distribution

1. Add the public key from our repository:

    $ sudo zypper --no-gpg-checks ar -f http://opendavinci.cse.chalmers.se/rpm-x86_64/repo OpenDaVINCI

2. Add our repository itself to your sources.list:

    $ sudo zypper --non-interactive --no-gpg-checks ar -f http://opendavinci.cse.chalmers.se/rpm-x86_64/repo OpenDaVINCI

3. Update your package database:

    $ sudo zypper --no-gpg-checks update

4. Install OpenDaVINCI:

   $ sudo zypper --no-gpg-checks install opendavinci-lib opendavinci-tools opendavinci-supercomponent
   
