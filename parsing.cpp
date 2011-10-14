/*
 * process.h
 *
 *  Created on: Oct 5, 2011
 *      Author: laufer
 */

#include <cstdlib> // atoi
#include <cstring> // strtok
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> // find, max

#include "parsing.h"
#include "process.h"

using std::find;
using std::max;
using std::string;
using std::vector;

typedef vector<string>::iterator IT;

const char* const TERM = " \t\n\r";
const char* const EOL = "\n\r";

size_t find_header_field(vector<string>& tokens, const char* const name, bool optional = false) {
	const IT first = tokens.begin(), last = tokens.end();
	const IT it = find(first, last, name);
	if (! optional && it == last) {
		std::cerr << "required header field " << name << " missing!" << std::endl;
		exit(1);
	}
	return it - first;
}

Format_Info::Format_Info(char* const header) {
	const char* tok = strtok(header, TERM);
	vector<string> tokens;
	while (tok != NULL) {
		tokens.push_back(tok);
		tok = strtok(NULL, TERM);
	}
	const int pid = find_header_field(tokens, "PID");
	const int ppid = find_header_field(tokens, "PPID");
	cmd = find_header_field(tokens, "CMD", true);
	if (cmd >= tokens.size())
		cmd = find_header_field(tokens, "COMMAND");
	if (cmd < max(pid, ppid)) {
		std::cerr << "CMD header must follow PID and PPID!" << std::endl;
		exit(1);
	}
	if (pid < ppid) {
		first = pid; second = ppid; pidFirst = true;
	} else {
		first = ppid; second = pid; pidFirst = false;
	}
}

void Format_Info::parse_process(process& p, char* const line) const {
	const char* tok = strtok(line, TERM);
	for (int i = 0; i < first; i++) tok = strtok(NULL, TERM);
	if (pidFirst) p.pid = atoi(tok); else p.ppid = atoi(tok);
	for (int i = first; i < second; i++) tok = strtok(NULL, TERM);
	if (pidFirst) p.ppid = atoi(tok); else p.pid = atoi(tok);
	for (int i = second; i < cmd - 1; i++) tok = strtok(NULL, TERM);
	p.cmd = strtok(NULL, EOL);
	// strip trailing newline if present
	size_t l = p.cmd.find('\n');
	if (l != string::npos) p.cmd = p.cmd.substr(0, int(l));
	l = p.cmd.find('\r');
	if (l != string::npos) p.cmd = p.cmd.substr(0, int(l));
}
