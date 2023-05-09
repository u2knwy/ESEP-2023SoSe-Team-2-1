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

class Options {
public:
	bool master;
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

        try {
            std::string mode = result["mode"].as<std::string>();
            if(mode != "master" && mode != "slave") {
                cerr << "Invalid value for first positional argument (must be 'master' or 'slave')" << endl;
                exit(EXIT_FAILURE);
            }
            master = (mode == "master");
        } catch (cxxopts::exceptions::option_has_no_value &ex) {
            cerr << "First positional argument is missing (must be 'master' or 'slave')." << endl;
            exit(EXIT_FAILURE);
        }

        master = result["master"].as<bool>();
        pusher = result["pusher"].as<bool>();

    }

};
