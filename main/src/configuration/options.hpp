/*
 * options.hpp
 *
 *  Created on: 08.05.2023
 *      Author: Maik
 */
#pragma once

#include "cxxopts.hpp"
#include <iostream>

using namespace std;

enum Mode { MASTER, SLAVE, TESTS, DEMO };

class Options {
public:
	Mode mode;
	bool pusher;

	Options(int argc, char **argv) {
		cxxopts::Options options("sorting-machine", "ESEP Sorting Machine");

		options.add_options()
					("mode", "Mode the system should be started as", cxxopts::value<std::string>())
					("p,pusher", "Pusher is mounted for sorting out workpieces (Default: switch is used)")

					("h,help", "Get help for usage");
		;
		options.allow_unrecognised_options();
		options.parse_positional({"mode"});

		auto result = options.parse(argc, argv);

		if (result.count("help"))
		{
			std::cout << options.help() << std::endl;
			exit(EXIT_SUCCESS);
		}

		std::string mode = result["mode"].as<std::string>();
		if(mode == "master") {
			this->mode = MASTER;
		} else if(mode == "slave") {
			this->mode = SLAVE;
		} else if(mode == "tests") {
			this->mode = TESTS;
		} else if(mode == "demo") {
			this->mode = DEMO;
		} else {
			throw cxxopts::exceptions::no_such_option(mode);
		}

		pusher = result["pusher"].as<bool>();
	}

};
