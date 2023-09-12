#include "Framework.h"
#include "Path.h"

string Path::Combine(vector<string> paths)
{
	string temp = "";
	for (string path : paths) {
		temp += path;
	}
	return temp;
}

wstring Path::Combine(vector<wstring> paths)
{
	wstring temp = L"";
	for (wstring path : paths) {
		temp += path;
	}
	return temp;
}

string Path::GetExtension(string path)
{
	String::Replace(&path, "\\", "/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}

wstring Path::GetExtension(wstring path)
{
	String::Replace(&path, L"\\", L"/");
	size_t index = path.find_last_of('.');

	return path.substr(index + 1, path.length());
}