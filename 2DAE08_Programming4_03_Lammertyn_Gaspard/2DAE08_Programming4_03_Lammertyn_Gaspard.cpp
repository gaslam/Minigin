// 2DAE08_Programming4_03_Lammertyn_Gaspard.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <vld.h>
#include <iomanip>
#include <filesystem>


struct Vector3 {
	float x, y, z;
};

#pragma region Exceptions
class FileNotFoundException {
public:
	FileNotFoundException(std::string message) : m_Message{ message } {};
	std::string Message() const { return m_Message; }
private:
	std::string m_Message{};
};
class CannotWriteDataException {
public:
	CannotWriteDataException(std::string message) : m_Message{ message } {};
	std::string Message() const { return m_Message; }
private:
	std::string m_Message{};
};
#pragma endregion


Vector3 GetVectorFromFileStreamInBinary(std::ifstream& in)
{
	Vector3 vec{};
	in.read(reinterpret_cast<char*>(&vec), sizeof(vec));
	return vec;
}

Vector3 GetVectorFromFileStreamIn(std::ifstream& in)
{
	float x, y, z;
	in >> x >> y >> z;
	return Vector3{ x,y,z };
}

//This code gave weird results when reading. I did not have time to fix them. vn unfortunately not supported and I know it's because of the char variable type
void ParseBOBJ(std::string& filePath, std::vector<Vector3>& normals, std::vector<Vector3>& faces, std::vector<Vector3>& verts, std::vector<std::string>& comments)
{
	std::ifstream in{ filePath,std::ios::binary };
	if (!in.is_open())
	{
		throw FileNotFoundException("Cannot find file: " + filePath + "!!");
	}

	std::string line;
	char command{};
	try
	{
		while (!in.eof())
		{
			command = ' ';
			in >> command;
			if (command == '#' && std::getline(in, line, static_cast<char>(0xFF)))
			{
				comments.push_back(line);
				continue;
			}

			if (command == 'v')
			{
				verts.emplace_back(GetVectorFromFileStreamInBinary(in));
				continue;
			}
			if (command == 'f')
			{
				faces.emplace_back(GetVectorFromFileStreamInBinary(in));
				continue;
			}
			if (command == 'vn')
			{
				normals.emplace_back(GetVectorFromFileStreamInBinary(in));
				continue;
			}
		}
		in.close();
	}
	catch (std::ifstream::failure& ex)
	{
		throw CannotWriteDataException("Cannot read/write data to file: " + filePath + "\nReason(s): " + std::string{ ex.what() } + "\n");
	}
}

void ParseOBJ(std::string& filePath, std::vector<Vector3>& normals, std::vector<Vector3>& faces, std::vector<Vector3>& verts, std::vector<std::string>& comments)
{
	std::ifstream in{ filePath };
	if (!in.is_open())
	{
		throw FileNotFoundException("Cannot find file: " + filePath + "!!");
	}

	std::string line;
	std::string command{};
	try
	{
		while (in.is_open())
		{
			command = "";
			in >> command;
			if (command == "#" && std::getline(in, line))
			{
				comments.emplace_back(line);
				continue;
			}

			if (command == "v")
			{
				verts.emplace_back(GetVectorFromFileStreamIn(in));
				continue;
			}
			if (command == "f")
			{
				faces.emplace_back(GetVectorFromFileStreamIn(in));
				continue;
			}
			if (command == "vn")
			{
				normals.emplace_back(GetVectorFromFileStreamIn(in));
				continue;
			}
			in.close();
		}
	}
	catch (std::ifstream::failure& ex)
	{
		throw CannotWriteDataException("Cannot read/write data to file: " + filePath + "\nReason(s): " + std::string{ ex.what() } + "\n");
	}
}

template <typename T>
void WriteToBinary(T& value, std::ofstream& out)
{
	out.write(reinterpret_cast<const char*>(&value), sizeof(float) * 3);
}

void WriteTextToBinary(const char* value, std::ofstream& out, size_t size)
{
	out.write(value, size);
}

template <typename T>
void AddDataBinary(std::string& command, std::vector<T>& data, std::ofstream& out)
{
	for (auto& object : data)
	{
		WriteTextToBinary(reinterpret_cast<const char*>(command.c_str()), out, command.size());
		WriteToBinary(object, out);
	}
}

void WriteToBOBJ(std::string& filePath, std::vector<Vector3>& normals, std::vector<Vector3>& faces, std::vector<Vector3>& verts, std::vector<std::string>& comments)
{
	std::ofstream out{ filePath,std::ios::binary };
	std::string command{ "#" };
	std::string valueToWrite{};
	if (!out.is_open())
	{
		throw FileNotFoundException("Cannot find file: " + filePath + "!!");
	}
	try
	{
		for (auto& comment : comments)
		{
			valueToWrite = command + comment;
			WriteTextToBinary(reinterpret_cast<const char*>(valueToWrite.c_str()), out, valueToWrite.size());
			out.put(static_cast<char>(0xFF));
		}

		command = "v";
		AddDataBinary(command, verts, out);

		command = "vn";
		AddDataBinary(command, normals, out);
		command = "f";
		AddDataBinary(command, faces, out);
		out.close();
	}
	catch (std::ifstream::failure& ex)
	{
		throw CannotWriteDataException("Cannot read/write data to file: " + filePath + "\nReason(s): " + std::string{ ex.what() } + "\n");
	}
}

//Reads a stream of data and adds it to the file stream.
//Directly writing it from a float or string does not make the file size different.
void AddVectorData(std::string& command, std::vector<Vector3>& data, std::ofstream& out)
{
	for (auto& vector : data)
	{
		//This needs to be here, because otherwise, the notation is also affected on whole numbers.
		//Let me know in feedback if I can improve this
		std::stringstream stream;
		stream << command << " ";
		if (static_cast<int>(vector.x) != vector.x)
		{
			stream << std::setprecision(7) << std::scientific;
		}

		//out << vector.x << " ";
		//out << vector.y << " ";
		//out << vector.z << "\n";
		stream << vector.x << " ";
		stream << vector.y << " ";
		stream << vector.z << "\n";
		out << stream.str();
	}
}

void WriteToOBJ(std::string& filePath, std::vector<Vector3>& normals, std::vector<Vector3>& faces, std::vector<Vector3>& verts, std::vector<std::string>& comments)
{
	std::ofstream out{ filePath };
	std::string command{ "#" };
	if (!out.is_open())
	{
		throw FileNotFoundException("Cannot find file: " + filePath + "!!");
	}

	try
	{
		for (auto& comment : comments)
		{
			out << "# " << comment << "\n";
		}
		command = "v";
		AddVectorData(command, verts, out);
		command = "vn";
		AddVectorData(command, normals, out);
		command = "f";
		AddVectorData(command, faces, out);
		out.close();
	}
	catch (std::ifstream::failure& ex)
	{
		throw CannotWriteDataException("Cannot read/write data to file: " + filePath + "\nReason(s): " + std::string{ ex.what() } + "\n");
	}
}


int main()
{
	std::vector<Vector3> normals, faces, verts;
	std::vector<std::string> comments;
	std::string filePathRead{ "Resources/low_poly_stanford_bunny.obj" };
	std::string filePathOutput{ "Resources/low_poly_stanford_bunny_output.obj" };
	std::string filePathBinary{ "Resources/low_poly_stanford_bunny.bObj" };

	std::cout << "=== OBJ to BOBJ converter ===\n\n";
	try {
		std::cout << "Reading data from obj... \nStatus message: ";
		ParseOBJ(filePathRead, normals, faces, verts, comments);
		std::cout << "OK!!\n\n";
		std::cout << "Writing data to binary obj... \nStatus message: ";
		WriteToBOBJ(filePathBinary, normals, faces, verts, comments);
		std::cout << "OK!!\n\n";

		const size_t totalNormals{ normals.size() };
		const size_t totalFaces{ faces.size() };
		const size_t totalVerts{ verts.size() };
		const size_t totalComments{ comments.size() };

		normals.clear();
		faces.clear();
		verts.clear();
		comments.clear();

		std::cout << "Reading data from binary obj... \nStatus message: ";
		ParseBOBJ(filePathBinary, normals, faces, verts, comments);
		std::cout << "OK!!\n\n";
		std::cout << "Writing data new obj... \nStatus message: ";
		WriteToOBJ(filePathOutput, normals, faces, verts, comments);
		std::cout << "OK!!\n\n";

		const size_t totalNewNormals{ normals.size() };
		const size_t totalNewFaces{ faces.size() };
		const size_t totalNewVerts{ verts.size() };
		const size_t totalNewComments{ comments.size() };

		std::cout << "Comparing results:\n\n Data:\n\n";
		std::cout << "Total vertex normals:\nBefore: " << totalNormals << "\nAfter: " << totalNewNormals << "\n\n";
		std::cout << "Total faces:\nBefore: " << totalFaces << "\nAfter: " << totalNewFaces << "\n\n";
		std::cout << "Total vertices:\nBefore: " << totalVerts << "\nAfter: " << totalNewVerts << "\n\n";
		std::cout << "Total comments:\nBefore: " << totalComments << "\nAfter: " << totalNewComments << "\n\n";

		std::cout << "The size of the original file is " << std::filesystem::file_size(filePathRead) << " bytes \n";
		std::cout << "The size of the new binary file is " << std::filesystem::file_size(filePathBinary) << " bytes \n";
		std::cout << "The size of the new file is " << std::filesystem::file_size(filePathOutput) << " bytes \n\n";

		std::cout << "Process done!!\n";
	}
	catch (FileNotFoundException& ex)
	{
		std::cout << ex.Message();
	}
	catch (CannotWriteDataException& ex)
	{
		std::cout << ex.Message();
	}
	std::cin.ignore();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
