#include <printf.h>
#include <syscall.h>

struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

using u32 = unsigned int;
using i32 = signed int;
using u64 = unsigned long;
using i64 = signed long;
using f64 = double;
using f32 = float;

void fill_rect(Pixel *fb, u32 x, u32 y, u32 width, u32 height, Pixel &color);
void set_pixel(Pixel *fb, u32 x, u32 y, Pixel &color);
void draw_cosine(Pixel *fb, u32 x, u32 y, u32 width, u32 height, Pixel &color);

int main()
{
	int VERSION = 1;
	printf("(%d): TESTING FRAMEBUFFER FROM USERSPACE\n", VERSION);
	Pixel *fb = (Pixel *)syscall_get_fb(6);
	Pixel ut_color = {255, 130, 0, 255};
	Pixel blue_color = {0, 0, 255, 255};
	fill_rect(fb, 15, 15, 250, 250, ut_color);
	draw_cosine(fb, 15, 200, 360, 100, blue_color);
	syscall_inv_rect(6, 0, 0, 640, 480);
	return 0;
}

void set_pixel(Pixel *fb, u32 x, u32 y, Pixel &color) {
	if (x < 640 && y < 480) {
		fb[y * 640 + x] = color;
	}
}

void fill_rect(Pixel *fb, u32 x, u32 y, u32 width, u32 height, Pixel &color) {
	for (u32 row = y; row < (y+height);row++) {
		for (u32 col = x; col < (x+width);col++) {
			set_pixel(fb, col, row, color);
		}
	}
}

f64 table_cos(f64 angle_degrees) {
	const f64 COS_TABLE[] = {
		1.0,
		0.9962,
		0.9848,
		0.9659,
		0.9397,
		0.9063,
		0.8660,
		0.8191,
		0.7660,
		0.7071,
		0.6428,
		0.5736,
		0.5000,
		0.4226,
		0.3420,
		0.2558,
		0.1736,
		0.0872,
		0.0,
		-0.0872,
		-0.1736,
		-0.2558,
		-0.3420,
		-0.4226,
		-0.5000,
		-0.5736,
		-0.6428,
		-0.7071,
		-0.7660,
		-0.8191,
		-0.8660,
		-0.9063,
		-0.9397,
		-0.9659,
		-0.9848,
		-0.9962,
		-1.0,
		-0.9962,
		-0.9848,
		-0.9659,
		-0.9397,
		-0.9063,
		-0.8660,
		-0.8191,
		-0.7660,
		-0.7071,
		-0.6428,
		-0.5736,
		-0.5000,
		-0.4226,
		-0.3420,
		-0.2558,
		-0.1736,
		-0.0872,
		0.0,
		0.0872,
		0.1736,
		0.2558,
		0.3420,
		0.4226,
		0.5000,
		0.5736,
		0.6428,
		0.7071,
		0.7660,
		0.8191,
		0.8660,
		0.9063,
		0.9397,
		0.9659,
		0.9848,
		0.9962,
		1.0,
	};
	u32 lookup_ang = angle_degrees / 5;
	return COS_TABLE[lookup_ang];
}

f64 mycos(f64 angle_degrees) {
	f64 x = 3.14159265359 * angle_degrees / 180.0;
	f64 result = 1.0;
	f64 inter = 1.0;
	f64 num = x * x;
	for (int i = 1;i <= 10;i++) {
		f64 comp = 2.0 * i;
		f64 den = comp * (comp - 1.0);
		inter *= num / den;
		if ((i % 2) == 0) {
			result += inter;
		}
		else {
			result -= inter;
		}
	}
	return result;
}

f64 mysin(f64 angle_degrees) {
	return mycos(90.0 - angle_degrees);
}

void draw_cosine(Pixel *fb, u32 x, u32 y, u32 width, u32 height, Pixel &color) {
	for (u32 i = 1; i <= width;i++) {
		f64 fy = -mycos(i % 360);
		f64 yy = fy * (height / 2.0);
		u32 nx = x + i;
		u32 ny = yy + y;
		// printf("Cos %u = %lf, x: %u, y: %u\n", (i % 360), fy, nx, ny);
		fill_rect(fb, nx, ny, 2, 2, color);
	}
}

