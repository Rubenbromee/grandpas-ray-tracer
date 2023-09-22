#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include "util.h"
#include "create_image.h"
#include "camera.h"

int main() {
	auto start = std::chrono::high_resolution_clock::now(); // Start the timer
	create_image(); // Render the image
	auto stop = std::chrono::high_resolution_clock::now(); // Stop the timer
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); // Calculate the render duration in milliseconds

	// Calculate hours, minutes and seconds of render duration
	int hours = duration.count() / 3600000;
	int minutes = (duration.count() % 3600000) / 60000;
	int seconds = (duration.count() % 60000) / 1000;

	// Output the duration in hours:minutes:seconds format
	std::cout << "Render time: ";
	std::cout << std::setw(2) << std::setfill('0') << hours << ":";
	std::cout << std::setw(2) << std::setfill('0') << minutes << ":";
	std::cout << std::setw(2) << std::setfill('0') << seconds << std::endl;

	return 0;
}