// COPYRIGHT (c) 2016 Alejandro Almira Molla <aalmiramolla at gmail dot com>
//
// GNU GENERAL PUBLIC LICENSE
//    Version 3, 29 June 2007
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <dirent.h> // For read directories

#include "face_detection/face_detection.hpp"
#include "face_detection/globals.hpp"

static void show_usage(std::string name) {
    std::cerr << "Usage: " << name << " <option(s)>" << std::endl
              << "Options:" << std::endl
              << "\t-h,--help\t\tShow this help message" << std::endl
              << "\t-c,--haarcascade FILE\t\tSpecify the path of the haarcascade" << std::endl
              << "\t-r,--recursive\t\tSearch in PATH recursively" << std::endl
              << "\t-D,--debug\t\tShow DEBUG messages" << std::endl
              << "\t-p,--path PATH\t\tSpecify the path of the images"
              << std::endl;
}

/** @function main */
int main( int argc, char** argv ) {

    if (argc < 4) {
        show_usage(argv[0]);
        return 1;
    }

    std::string path;
    std::string face_cascade_name;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-p") || (arg == "--path")) {
            if (i + 1 < argc) {
                path = argv[++i];
            } else {
                std::cerr << "--path option requires one argument." << std::endl;
                return -1;
            }
        } else if ((arg == "-c") || (arg == "--haarcascade")) {
            if (i + 1 < argc) {
                face_cascade_name = argv[++i];
            } else {
                std::cerr << "--haarcascade option requires one argument." << std::endl;
                return -1;
            }
        } else if ((arg == "-r") || (arg == "--recursive")) {
            T_RECURSIVE = true;
        } else if ((arg == "-D") || (arg == "--debug")) {
            T_DEBUG = true;
        }
    }

	FaceDetection face_detection(face_cascade_name);
	face_detection.obtain_images(path);

    if (face_detection.get_images().empty()) {
        std::cerr << "ERROR: No images on " << path << std::endl;
        return -1;
    }
	std::cout << "Images size:" << face_detection.get_images().size() << "\n";
    std::vector<cv::Mat> faces = process_images(face_detection);
    // create_mosaic(faces);
    save_faces(faces, "./faces/");
    return 0;
}
