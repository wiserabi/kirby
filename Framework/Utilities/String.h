#pragma once


class String
{
public:
	static void SplitString(vector<string>* result, string origin, string tok);
	static void SplitString(vector<wstring>* result, wstring origin, wstring tok);

	static bool Startwith(string str, string comp);
	static bool Startwith(wstring str, wstring comp);

	static bool Contain(string str, string comp);
	static bool Contain(wstring str, wstring comp);


	static void Replace(string* str, string comp, string rep);
	static void Replace(wstring* str, wstring comp, wstring rep);

	static string ToString(Vector3 vec3, int precision=5);
	static wstring ToWString(Vector3 vec3, int precision = 5);

	static string ToString(wstring str);
	static wstring ToWString(string str);
};