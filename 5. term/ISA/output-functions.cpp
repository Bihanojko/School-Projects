// Subject: Network Applications and Network Administration
// Project: Monitoring of DHCP Communication
// Author: Nikola Valesova
// Login: xvales02
// Date: 19. 11. 2016
// output-functions.cpp


#include "dhcp-stats.h"


// function that prints output to log file ("-c" parameter)
int print_to_log_file()
{
    // open log file for writing to create it or delete previous content
    FILE *log_file;
    if ((log_file = fopen("logfile.csv", "w")) == NULL)
    {
        cerr << "Opening log file failed!" << endl;
        exit(4);
    }
    fclose(log_file);

    while (true)
    {
        // open log file again to append
        if ((log_file = fopen("logfile.csv", "a")) == NULL)
        {
            cerr << "Opening log file failed!" << endl;
            exit(4);
        }
    
        // get current time
        time_t now = time(0);
        struct tm tstruct = *localtime(&now);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        // print a timestamp to log file
        fprintf(log_file, "%s\n", buf);

        // print the whole table of statistics
        fputs("IP Prefix,Max hosts,Allocated addresses,Utilization\n", log_file);
        for (vector <int>::size_type i = 0; i != subnet_list.IP_address.size(); i++)
        {
            string value = to_string(subnet_list.IP_address[i][0]) + "." + to_string(subnet_list.IP_address[i][1]) +
            "." + to_string(subnet_list.IP_address[i][2]) + "." +  to_string(subnet_list.IP_address[i][3]) + "/" + 
            (to_string(subnet_list.mask[i])).c_str() + ",";
            fputs(value.c_str(), log_file);
            fputs(to_string(subnet_list.max_hosts[i]).c_str(), log_file);
            fputs(",", log_file);
            fputs(to_string(subnet_list.current_hosts[i]).c_str(), log_file);
            fputs(",", log_file);
            int ratio = round(((float)subnet_list.current_hosts[i]) * 10000 / (float)subnet_list.max_hosts[i]);
            fprintf(log_file, "%g%%\n", ((float)ratio / 100));
        }
        fprintf(log_file, "\n");
        fclose(log_file);

        // wait until next writing
        sleep(interval);
    }
    return 0;
}


// funtion that prints output to terminal using ncurses library functions 
void print_output()
{
    // if "-c" parameter set, do not write to terminal
    if (log_file)
        return;

    // initialize screen
    initscr();
    clear();

    // print name of program and inserted arguments
    mvprintw(0, 0, "./dhcp-stats ");
    for (vector <int>::size_type i = 0; i != subnet_list.IP_address.size(); i++)
        printw("%u.%u.%u.%u/%d ", subnet_list.IP_address[i][0], subnet_list.IP_address[i][1], 
        subnet_list.IP_address[i][2], subnet_list.IP_address[i][3], subnet_list.mask[i]);

    // print the whole table of statistics
    mvprintw(2, 0, "IP Prefix");
    mvprintw(2, 20, "Max hosts");
    mvprintw(2, 40, "Allocated addresses");
    mvprintw(2, 65, "Utilization");

    for (vector <int>::size_type i = 0; i != subnet_list.IP_address.size(); i++)
    {
        mvprintw(3 + i, 0, "%u.%u.%u.%u/%d",subnet_list.IP_address[i][0], subnet_list.IP_address[i][1], 
            subnet_list.IP_address[i][2], subnet_list.IP_address[i][3], subnet_list.mask[i]);
        mvprintw(3 + i, 20, "%s",to_string(subnet_list.max_hosts[i]).c_str());
        mvprintw(3 + i, 40, "%s",to_string(subnet_list.current_hosts[i]).c_str());
        int ratio = round(((float)subnet_list.current_hosts[i]) * 10000 / (float)subnet_list.max_hosts[i]);
        mvprintw(3 + i, 65, "%g %%", ((float)ratio / 100));
    }
    
    refresh();
    return;
}


// function that prints help to stdout and exits program
void print_help()
{
    cout << "Monitorování DHCP komunikace" << endl;
    cout << "Použití:" << endl;
    cout << "./dhcp-stats <ip-prefix> [ <ip-prefix> [ ... ] ] [-c <int>]" << endl;
    cout << "Program monitoruje vytížení síťového prefixu a vytváří ze získaných údajů statistiky." << endl;
    cout << "Přepínač -c způsobí vypsání aktuální statistiky o vytížení do souboru logfile.csv, a to každých int sekund." << endl;

    exit(0);
}