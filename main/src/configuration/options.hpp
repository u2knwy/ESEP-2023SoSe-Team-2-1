/*
 * options.hpp
 *
 *  Created on: 08.05.2023
 *      Author: Maik
 */
#pragma once

#include "cxxopts.hpp"
#include <iostream>

class Options {
public:
	bool slave;
    bool pusher;

    Options(int argc, char **argv) {
        cxxopts::Options options("sortingmachine", "ESEP Sorting Machine");

        options.add_options()
            ("s,slave", "Start program as Slave (true) or Master (false). Default: Master",
            		cxxopts::value(slave)->default_value("false"))
			("p,pusher", "Type of sorter is 'Pusher'. Otherwise it is 'Switch'",
					cxxopts::value(pusher)->default_value("false"))

            //info
            ("h,help", "Get help for usage")
		;

        auto result = options.parse(argc, argv);

        if (result.count("help"))
		{
		  std::cout << options.help() << std::endl;
		  exit(EXIT_SUCCESS);
		}

        slave = result["slave"].as<bool>();
        pusher = result["pusher"].as<bool>();

    }

};
