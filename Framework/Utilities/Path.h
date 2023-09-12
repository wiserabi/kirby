#pragma once

#include <functional>

class Path
{
public:
	static string Combine(vector<string> paths);
	static wstring Combine(vector<wstring> paths);

	static string GetExtension(string path);
	static wstring GetExtension(wstring path);


};