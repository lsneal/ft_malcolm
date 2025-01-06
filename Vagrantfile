$prerequisite = <<-SCRIPT
	sudo yum install clang figlet ruby zip -y
	cd /tmp && wget https://github.com/busyloop/lolcat/archive/master.zip
	unzip master.zip && cd lolcat-master
	gem install lolcat
SCRIPT

Vagrant.configure("2") do |config|
	config.vm.box = "generic/centos8"
	config.vm.synced_folder ".", "/tmp/ft_malcom"

	config.vm.define "blackhat" do |c|
		c.vm.box = "generic/centos8"
		c.vm.hostname = "blackhat"
		c.vm.network "private_network", ip: "192.168.56.2"

		c.vm.provider "virtualbox" do |vb|
			vb.customize ["modifyvm", :id, "--natdnsproxy1", "on"]
			vb.customize ["modifyvm", :id, "--natdnshostresolver1", "on"]
			vb.customize ["modifyvm", :id, "--nictrace2", "on", "--nictracefile2", "blackhat.pcap"]
		end

		c.vm.provision "shell",
			inline: $prerequisite
	end

	config.vm.define "target1" do |c|
		c.vm.box = "generic/centos8"
		c.vm.hostname = "target1"
		c.vm.network "private_network", ip: "192.168.56.3"

		c.vm.provider "virtualbox" do |vb|
			vb.customize ["modifyvm", :id, "--natdnsproxy1", "on"]
			vb.customize ["modifyvm", :id, "--natdnshostresolver1", "on"]
			vb.customize ["modifyvm", :id, "--nictrace2", "on", "--nictracefile2", "target1.pcap"]
		end
	end
	
	config.vm.define "target2" do |c|
		c.vm.box = "generic/centos8"
		c.vm.hostname = "target2"
		c.vm.network "private_network", ip: "192.168.56.4"

		c.vm.provider "virtualbox" do |vb|
			vb.customize ["modifyvm", :id, "--natdnsproxy1", "on"]
			vb.customize ["modifyvm", :id, "--natdnshostresolver1", "on"]
			vb.customize ["modifyvm", :id, "--nictrace2", "on", "--nictracefile2", "target2.pcap"]
		end
	end
end