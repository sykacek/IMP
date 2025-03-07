/**
 * Hobby rocket altitude measuring software
 * Syntax: ./exe file.bin (-g a1 a2 a3)
 * -g forces to use the specified gravity values
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <fstream>
#include <string>
#include <math.h>

#include "matrix.h"
#include "vector.h"
#include "defines.h"

/* Defines */
#define BIT_RESOLUTION				(16)
#define LSB_G(resolution)			((float)(1 << BIT_RESOLUTION) / (2 * resolution))
#define LSB_DEGREE(resolution)			((float)(1 << BIT_RESOLUTION) / (2 * resolution))

#define DEGREE_TO_RAD(x)			(((float)x) * M_PI / 180)
#define RAW_TO_ACCEL(x, resolution)		(((float)x)/ LSB_G(resolution))
#define RAW_TO_GYRO(x, resolution)		(((float)DEGREE_TO_RAD(x))/ LSB_DEGREE(resolution))

#define BIN_OFFSET				(0x42000)

#define STREAM_ARRAY(stream, arr)		(stream << std::setprecision(FSTREAM_PRECISION) << arr[0] << " " \
						<< arr[1] << " " << arr[2] << "\n")

/* NVS page formated struct */
typedef struct page {
	char header[16];
	int16_t data[120];
} page_t;

const unsigned char header_id[16] = NVS_HEADER_IDENTIFIER;
static vect_t gravity(1.0f, 0.0f, 0.0f);

static uint16_t G_BIT_RESOLUTION,
		DEGREE_BIT_RESOLUTION;

int main(int argc, char **argv){
	if(argc < 2){
		std::cerr << "Error: no input file specified\n";
		exit(1);
	} else if(argc > 2 && argc < 5){
		std::cerr << "Error, bad number of arguments, aborting\n";
		exit(1);
	}

	FILE *read = fopen(argv[1], "rb");

	std::ofstream posfs(POSITION_OUTPUT_FILE, std::ios_base::out),
		velfs(VELOCITY_OUTPUT_FILE, std::ios_base::out),
		accfs(ACCEL_OUTPUT_FILE, std::ios_base::out),
		gyrfs(GYRO_OUTPUT_FILE, std::ios_base::out);

#ifdef DEBUG
	std::ofstream debugfs(DEBUG_CONFIG_FILE, std::ios_base::out),
		sinfs(SIN_OUTPUT_FILE, std::ios_base::out),
		cosfs(COS_OUTPUT_FILE, std::ios_base::out);
#endif

	page_t page;
	char name[64];
	vect_t v(0.0f, 0.0f, 0.0f), s(0.0f, 0.0f, 0.0f), a;
	rot_t rotation[3];
	float alpha[3] = {0};
	float temp[3];
	uint32_t index = 0,
	lowBound, highBound;

	rotation[0].update_dir(X_ROTATION);
	rotation[1].update_dir(Y_ROTATION);
	rotation[2].update_dir(Z_ROTATION);

	/* get the data resolution */
	fseek(read, BIN_OFFSET + 128, SEEK_SET);
	fread(name, 1, 16, read);
	if(strcmp(name, GYRO_RANGE_125_STR) == 0){
		DEGREE_BIT_RESOLUTION = 125;
	} else if(strcmp(name, GYRO_RANGE_250_STR) == 0){
		DEGREE_BIT_RESOLUTION = 250;
	} else if(strcmp(name, GYRO_RANGE_500_STR) == 0){
		DEGREE_BIT_RESOLUTION = 500;
	} else if(strcmp(name, GYRO_RANGE_1000_STR) == 0){
		DEGREE_BIT_RESOLUTION = 1000;
	} else if(strcmp(name, GYRO_RANGE_2000_STR) == 0){
		DEGREE_BIT_RESOLUTION = 2000;
	} else {
		fprintf(stderr, "gyro header out of range (bad file format), aborting\n");
		exit(1);

	}
#ifdef DEBUG
	debugfs << "found header " << name << "\nusing gyroscope resosultion " << DEGREE_BIT_RESOLUTION << "\n";
#endif

	fseek(read, BIN_OFFSET + 160, SEEK_SET);
	fread(name, 1, 16, read);
	if(strcmp(name, ACCEL_RANGE_2G_STR) == 0){
		G_BIT_RESOLUTION = 2;
	} else if(strcmp(name, ACCEL_RANGE_4G_STR) == 0){
		G_BIT_RESOLUTION = 4;
	} else if(strcmp(name, ACCEL_RANGE_8G_STR) == 0){
		G_BIT_RESOLUTION = 8;
	} else if(strcmp(name, ACCEL_RANGE_16G_STR) == 0){
		G_BIT_RESOLUTION = 16;
	} else {
		fprintf(stderr, "accel header out of range (bad file format), aborting\n");
		exit(1);
	}

#ifdef DEBUG
	debugfs << "found header " << name << "\nusing accelometer resosultion " << G_BIT_RESOLUTION << "\n";
#endif
	/* specify data processing range */
	fseek(read, BIN_OFFSET + 0x100, SEEK_SET);
	while(1){
		fread(&page, 256, 1, read);
		memcpy(name, &page, 15);
		if(memcmp(header_id, name, 15) != 0)
			break;
		
		for(int i = 0; i < 20; ++i){
			std::cout << index++ << " " << RAW_TO_ACCEL(page.data[i * 6 + 2], G_BIT_RESOLUTION) << "\n";
		}
	}	

	/* try to read range */
	std::cout << "please enter a starting index (divisible by 20)\n";
	std::string str;
	std::cin >> str;

	try {
		lowBound = std::stoi(str);
	} catch(const std::invalid_argument& e){
		std::cerr << "error, invalid argument, aborting\n";
		exit(1);
	} catch (const std::out_of_range& e){
		std::cerr << "overflow detected, aborting\n";
		exit(1);
	}

	if(lowBound < 0 || lowBound > index){
		std::cerr << "error, value is out of range, aborting\n";
		exit(1);
	} else if(lowBound % 20 != 0){
		std::cerr << "error, value IN NOT DIVISIBLE BY 20, aborting\n";
		exit(1);
	}

	std::cout << "please enter end index (divisible by 20)\n";
	std::cin >> str;
	try {
		highBound = std::stoi(str);
	} catch(const std::invalid_argument& e){
		std::cerr << "error, invalid argument, aborting\n";
		exit(1);
	} catch (const std::out_of_range& e){
		std::cerr << "overflow detected, aborting\n";
		exit(1);
	}

	if(lowBound > highBound || highBound > index){
		std::cerr << "error, value is out of range, aborting\n";
		exit(1);
	} else if(highBound % 20 != 0){
		std::cerr << "error, value IN NOT DIVISIBLE BY 20, aborting\n";
		exit(1);
	}

	/* first read to get gravity */
	fseek(read, BIN_OFFSET + 0x100 * (1 + lowBound / 20), SEEK_SET);
	fread(&page, 256, 1, read);
	memcpy(name, &page, 15);

	/* get gravity vector */
	for(int j = 0; j < 3; ++j){
		temp[j] = ((float)page.data[j]) / LSB_G(G_BIT_RESOLUTION);
	}

	if(argc > 2 && argv[2][0] == '-' && argv[2][1] == 'g'){
		std::cout << "forcing gravity\n";
		for(int i = 0; i < 3; ++i){
			try{
				temp[i] = std::stof(argv[3 + i]);
			} catch (const std::invalid_argument& e){
				std::cerr << "error, invalid argument (gravity), aborting";
				exit(1);
			} catch (const std::out_of_range& e){
				std::cerr << "error, argument out of range (gravity), aborting";
				exit(1);
			}
		}
	}

	gravity.update(temp);
	gravity*= (-1.0f);
	gravity.round(2);

#ifdef DEBUG
	debugfs << "gravity vector: " << gravity;
#endif

	index = 0;
	while(index < highBound){
		while(lowBound / 20){
			index += 20;
			lowBound -= 20;
			fread(&page, 256, 1, read);
		}

		fread(&page, 256, 1, read);
		memcpy(name, &page, 15);

		/* continue only if header matches */
		if(memcmp(header_id, name, 15) != 0)
			break;

		for(int i = 0; i < 20; ++i){
			/* get accelometer data */
			for(int j = 0; j < 3; ++j)
				temp[j] = RAW_TO_ACCEL(page.data[i * 6 + j], G_BIT_RESOLUTION);

			a.update(temp);
			//std::cout << index << " " << a.m[2] << "\n";
			accfs << index << " " << a;

			/* get gyroscope data */
			for(int j = 0; j < 3; ++j){
				temp[j] = RAW_TO_GYRO(page.data[i * 6 + 3 + j], DEGREE_BIT_RESOLUTION);
				alpha[j] += temp[j];
 
				rotation[j].update(alpha[j]);
//				rotation[j].update(temp[j]);
			}
			gyrfs << index  << " ";
			STREAM_ARRAY(gyrfs, temp);

#ifdef DEBUG
			cosfs << cosf(temp[0]) << " " << cosf(temp[1]) << " " << cosf(temp[2]) << "\n";
			sinfs << sinf(temp[0]) << " " << sinf(temp[1]) << " " << sinf(temp[2]) << "\n";
#endif

			rotation[0] *= rotation[1];
			rotation[0] *= rotation[2];

			a = rotation[0] * a;
			a.round(2);
			a += gravity;

			v += (a * 0.1f);
			s += (a * 0.01f);

			posfs << index << " " << s;
			velfs << index << " " << v;
			index++;
		}
	}

	posfs.close();
	velfs.close();
	accfs.close();
	gyrfs.close();

#ifdef DEBUG
	debugfs.close();
	sinfs.close();
	cosfs.close();
#endif

	fclose(read);
	std::cout << "v is\n" << v << "\ns is\n" << s;

	return 0;
}