/**
 * @file		XMLfiles.h
 *	@author		Nahum Budin
 *	@date		24-Oct-2025
 *	@version	1.2
 *					1. Adding function get_xml_file_path to extract path from full path
 *
 *	@brief	XML files handling (for settings and configuration files).
 *
 *  Based on Music Open Lab Library.
 *	Copyright AdjHeart Nahum Budin Octobe 1, 2018
 *
 *	History:\n
 *
 *	version 1.1		29-Jun-2024: Code refactoring
 *	version 1.0		17-Oct-2019:
 *		First version
 */

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <iterator>


using namespace std;

class XML_files
{
  public:
	XML_files();
	/**
	*	Returns leading-spaces x indent<tag> and optionally a new line
	*/
	std::string start_tag(std::string tag, int indent = 0, bool newline = false);
	/**
	*	Returns leading-spaces x indent</tag> and a new line
	*/
	std::string end_tag(std::string tag, int indent = 0);
	/**
	*	Returns leading-spaces x indent<tag>name=int val</tag> and new line
	*/
	std::string int_val_element(std::string tag, std::string name, int val, int indent = 0);
	/**
	*	Returns leading-spaces x indent<tag>name=bool val</tag> and new line
	*/
	std::string bool_val_element(std::string tag, std::string name, bool val, int indent = 0);
	/**
	*	Returns leading-spaces x indent<tag>name=float val</tag> and new line
	*/
	std::string float_val_element(std::string tag, std::string name, float val, int indent = 0);
	/**
	*	Returns leading-spaces x indent<tag>name=string val</tag> and new line
	*/
	std::string string_val_element(std::string tag, std::string name, std::string val, int indent = 0);
	/**
	*	Returns <!--comment-->
	*/
	std::string comment(std::string comment);
	/**
	*	Scans an XML file text loking for a tag and reurns true if found
	*/
	bool element_exist(const string &xmltext, string tag);

	// Extract and build elements vectors per type
	/**
	*	Returns a vector of std::string elements of all "int_param" tag
	*/
	vector<std::string> get_int_params(const std::string &xmltext);
	/**
	*	Returns a vector of std::string elements of all "bool_param" tag
	*/
	vector<std::string> get_bool_params(const std::string &xmltext);
	/**
	*	Returns a vector of std::string elements of all "float_param" tag
	*/
	vector<std::string> get_float_params(const std::string &xmltext);
	/**
	*	Returns a vector of std::string elements of all "string_param" tag
	*/
	vector<std::string> get_string_params(const std::string &xmltext);
	/**
	*	Returns an std::string containing an element name value
	*/
	std::string get_element_name(std::string element);
	/**
	*	Returns an std::string containing an element value
	*/
	std::string get_element_value(std::string element);
	/**
	*	Returns an int value contained in an int-element value
	*/
	int get_int_element_value(std::string element);
	/**
	*	Returns a bool value contained in a bool-element value
	*/
	bool get_bool_element_value(std::string element);
	/**
	*	Returns a float value contained in a float-element value
	*/
	float get_float_element_value(std::string element);
	/**
	*	Returns an std::string value contained in a string-element value
	*/
	std::string get_string_element_value(std::string element);
	/**
	* Read an XML file into an std::string filedata
	*/
	int read_xml_file(const char *fullpath = NULL, std::string *filedata = NULL);
	/**
	* write an vector<string> XML elements text filedata into an XML file (removes file extension and adds .xml)
	*/
	int write_xml_file(const char *fullpath = NULL, vector<string> *filedata = NULL);
	/**
	* Extract file name from full path 
	*/
	std::string get_xml_file_name(std::string fullpath);
	/**
	 * Extract file path without name and extenssion from full path
	 */
	std::string get_xml_file_path(std::string fullpath);
	/**
	* Extract file name without the extension
	*/
	std::string remove_file_extention(std::string file_name);

  private:
	// Build a name=(type)value strings
	/**
	*	Returns name=int val
	*/
	std::string int_val_attribute(std::string name, int val);
	/**
	*	Returns name=bool val
	*/
	std::string bool_val_attribute(std::string name, bool val);
	/**
	*	Returns name=float val
	*/
	std::string float_val_attribute(std::string name, float val);
	/**
	*	Returns name=string val
	*/
	std::string string_val_attribute(std::string name, string val);
	/**
	*	Returns a vector of std::string elements of a given tag found in xmltext
	*/
	vector<std::string> get_elements(const std::string &xmltext, std::string tag);

	//	static char patches_dir[256];
	//	static char sketches_dir[256];
};


