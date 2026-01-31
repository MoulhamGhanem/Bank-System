#pragma once
#include <iostream>
#include "clsString.h"
#include <vector>
#include <fstream>
#include <string>

class clsCurrency
{
private:
	enum enMode { EmptyMode = 0, UpdatedMode = 1 };
	enMode _Mode;
	string _Country;
	string _CurrencyCode;
	string _CurrencyName;
	float _Rate;

	static clsCurrency _ConvertLineToCurrencyObject(string Line,string Seperator="#//#")
	{
		vector<string> vCurrencyData;
		vCurrencyData = clsString::Split(Line,Seperator);
		return clsCurrency(enMode::UpdatedMode,  vCurrencyData[0],
			vCurrencyData[1], vCurrencyData[2], stof(vCurrencyData[3]));


	}

	static string _ConvertCurrencyObjectToLine(clsCurrency Currency, string Seperator = "#//#")
	{
		string CurrencyData = "";
		CurrencyData += Currency.Country() + Seperator;
		CurrencyData += Currency.CurrencyCode() + Seperator;
		CurrencyData += Currency.CurrencyName() + Seperator;
		CurrencyData += to_string(Currency.Rate()) + Seperator;

		return CurrencyData;

	}

	static vector<clsCurrency> _LoadCurrencyDataFromFile()
	{
		vector<clsCurrency> vCurrencies;
		fstream MyFile;
		MyFile.open("Currencies.txt",ios::in);
		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsCurrency CurrencyData = _ConvertLineToCurrencyObject(Line);
				vCurrencies.push_back(CurrencyData);
			}
			MyFile.close();

		}
		return vCurrencies;
	}

	static void _SaveCurrencyDataToFile(vector<clsCurrency> vCurrencies)
	{
		fstream MyFile;
		MyFile.open("Currencies.txt", ios::out);
		if (MyFile.is_open())
		{
			for (clsCurrency& Currency : vCurrencies)
			{
				string Line = _ConvertCurrencyObjectToLine(Currency);
				MyFile << Line << endl;
			}

		}
		MyFile.close();
		
	}

	void _Update()
	{
		vector<clsCurrency> _vCurrencies;
		_vCurrencies = _LoadCurrencyDataFromFile();
		for (clsCurrency& C : _vCurrencies)
		{
			if (C.CurrencyCode() == CurrencyCode())
			{
				C = *this;
				break;
			}
		}
		_SaveCurrencyDataToFile(_vCurrencies);
	}

	static clsCurrency _GetEmptyCurrencyObject()
	{
		return clsCurrency(enMode::EmptyMode, "", "", "", 0);
	}



public:
	clsCurrency(enMode Mode, string Country, string CurrencyCode, string CurrencyName, float Rate)
	{
		_Mode = Mode;
		_Country = Country;
		_CurrencyCode = CurrencyCode;
		_CurrencyName = CurrencyName;
		_Rate = Rate;
	}

	static vector <clsCurrency> GetAllUSDRates()
	{

		return _LoadCurrencyDataFromFile();

	}

	bool IsEmpty()
	{
		return _Mode == enMode::EmptyMode;
	}

	string Country()
	{
		return _Country;
	}

	string CurrencyCode()
	{
		return _CurrencyCode;
	}

	string CurrencyName()
	{
		return _CurrencyName;
	}

	void UpdateRate(float NewRate)
	{
		_Rate = NewRate;
		_Update();
	}

	float Rate()
	{
		return _Rate;
	}

	static clsCurrency FindByCode(string CurrencyCode)
	{
		CurrencyCode = clsString::UpperAllString(CurrencyCode);

		fstream MyFile;
		MyFile.open("Currencies.txt", ios::in);
		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsCurrency Currency = _ConvertLineToCurrencyObject(Line);
				if (Currency.CurrencyCode() == CurrencyCode)
				{
					MyFile.close();
					return Currency;
				}
			}
			MyFile.close();
			return _GetEmptyCurrencyObject();
		}
	}

	static clsCurrency FindByCountry(string Country)
	{
		Country = clsString::UpperAllString(Country);

		fstream MyFile;
		MyFile.open("Currencies.txt", ios::in);
		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsCurrency Currency = _ConvertLineToCurrencyObject(Line);
				if (Currency.Country() == Country)
				{
					MyFile.close();
					return Currency;
				}
			}
			MyFile.close();
			return _GetEmptyCurrencyObject();
		}
	}

	static bool IsCurrencyExist(string CurrencyCode)
	{
		clsCurrency Currency = FindByCode(CurrencyCode);
		return (!Currency.IsEmpty());
	}

	static vector <clsCurrency> GetCurrenciesList()
	{
		return _LoadCurrencyDataFromFile();
	}
	

	float ConvertToUSD(float Amount)
	{
		return (float)(Amount / Rate());
	}

	float ConvertToOtherCurrency(float Amount, clsCurrency Currency2)
	{
		float AmountInUSD = ConvertToUSD(Amount);

		if (Currency2.CurrencyCode() == "USD")
		{
			return AmountInUSD;
		}

		return (float)(AmountInUSD * Currency2.Rate());

	}

};

