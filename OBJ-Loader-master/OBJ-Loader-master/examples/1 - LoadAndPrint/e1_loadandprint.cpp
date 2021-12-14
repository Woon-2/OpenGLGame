// Example 1: Load and Print
//
// Load the data from the .obj then print it into a file
//	called e1Out.txt

// Iostream - STD I/O Library
#include <iostream>

// fStream - STD File I/O Library
#include <fstream>

// OBJ_Loader - .obj Loader
#include "../../Source/OBJ_Loader.h"

// Main function
int main(int argc, char* argv[])
{
	// Initialize Loader
	objl::Loader Loader;

	// Load .obj File
	bool loadout = Loader.LoadFile("resources/object/obj/Space Station Scene.obj");

	// Check to see if it loaded

	// If so continue
	if (loadout)
	{
		// Go through each loaded mesh and out its contents
		for (int i = 0; i < Loader.LoadedMeshes.size(); i++)
		{
			// Copy one of the loaded meshes to be our current mesh
			objl::Mesh curMesh = Loader.LoadedMeshes[i];
			std::ofstream mesh{ std::string{"resources/object/loaded/"} + curMesh.MeshName + std::string{".obj"} };

			// Go through each vertex and print its number,
			//  position, normal, and texture coordinate
			for (int j = 0; j < curMesh.Vertices.size(); j++)
			{
				mesh << "v " << curMesh.Vertices[ j ].Position.X << ' ' << curMesh.Vertices[ j ].Position.Y << ' ' << curMesh.Vertices[ j ].Position.Z << '\n';
			}
			mesh << '\n';

			for ( int j = 0; j < curMesh.Vertices.size(); j++ )
			{
				mesh << "vn " << curMesh.Vertices[ j ].Normal.X << ' ' << curMesh.Vertices[ j ].Normal.Y << ' ' << curMesh.Vertices[ j ].Normal.Z << '\n';
			}
			mesh << '\n';

			for ( int j = 0; j < curMesh.Vertices.size(); j++ )
			{
				mesh << "vt " << curMesh.Vertices[ j ].TextureCoordinate.X << ' ' << curMesh.Vertices[ j ].TextureCoordinate.Y << '\n';
			}
			mesh << '\n';

			// Go through every 3rd index and print the
			//	triangle that these indices represent
			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				mesh << "f "
					<< curMesh.Indices[ j ] + 1 << '/' << curMesh.Indices[ j ] + 1 << '/' << curMesh.Indices[ j ] + 1 << ' '
					<< curMesh.Indices[ j + 1 ] + 1 << '/' << curMesh.Indices[ j + 1 ] + 1 << '/' << curMesh.Indices[ j + 1 ] + 1 << ' '
					<< curMesh.Indices[ j + 2 ] + 1 << '/' << curMesh.Indices[ j + 2 ] + 1 << '/' << curMesh.Indices[ j + 2 ] + 1 << '\n';
			}
			mesh << '\n';

			mesh << "# " << "A " << curMesh.MeshMaterial.Ka.X << " " << curMesh.MeshMaterial.Ka.Y << " " << curMesh.MeshMaterial.Ka.Z << "\n";
			mesh << "# " << "D " << curMesh.MeshMaterial.Kd.X << " " << curMesh.MeshMaterial.Kd.Y << " " << curMesh.MeshMaterial.Kd.Z << "\n";
			mesh << "# " << "S " << curMesh.MeshMaterial.Ks.X << " " << curMesh.MeshMaterial.Ks.Y << " " << curMesh.MeshMaterial.Ks.Z << "\n";
			mesh << "# " << "E " << curMesh.MeshMaterial.Ns << "\n";
			mesh << "# " << "AT " << curMesh.MeshMaterial.map_Ka << "\n";
			mesh << "# " << "DT " << curMesh.MeshMaterial.map_Kd << "\n";
			mesh << "# " << "ST " << curMesh.MeshMaterial.map_Ks << "\n";
			mesh << "# " << "PT " << curMesh.MeshMaterial.map_d << "\n";
		}

	}
	// If not output an error
	else
	{
		// Create/Open e1Out.txt
		std::ofstream file("e1Out.txt");

		// Output Error
		file << "Failed to Load File. May have failed to find it or it was not an .obj file.\n";

		// Close File
		file.close();
	}

	// Exit the program
	return 0;
}