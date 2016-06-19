#include "fixedPoint.h"

#define INTEGER_PART 0xFFFF0000 //The Bits in the integer part.
#define SIGN_BIT 0x80000000 //The Sign Bit of the integer part.
#define FRACTION_PART 0x0000FFFF //The Bits in the fraction part.
#define SHIFT 16 //The number of bits that a Fixed Point value is shifted.

#define REVOLUTION 0x01000000 //Angle of a full revolution
#define HALF_CIRCLE 0x00800000 //Angle of half circle.
#define QUARTER_CIRCLE 0x00400000 //Angle of a quarter of a circle.
#define CIRCLE_BITS 0x00FFFFFF //The bits used in the definition of a single circle.
#define REVOLUTION_BITS 0xFF000000 //The bits that are used to define the number of revolutions in an angle.
#define REVOLUTION_SHIFT_INT 24 //The shift to get the number of revolutions in an angle as an integer.
#define REVOLUTION_SHIFT_FIX REVOLUTION_SHIFT_INT - SHIFT //The shift to get the number of revolutions in an angle as a fixed point number.

fixed Fixed_Point::mul(fixed left, fixed right) {
	__asm {
		mov eax, left //Move left into eax.
		imul right; //left * right
		shrd eax, edx, SHIFT; //Shift the bits to the right because the
		//multiplication automatically rotates it left.
	}
}

fixed Fixed_Point::div(fixed left, fixed right) {
	__asm {
		mov eax, left; //Position the numerator(left) in eax.
		cmp eax, 0; //	if (left >= 0) {
		jge els; //			if left >= 0 goto els
		mov edx, -1; //		if left < 0 edx is filled with 1's
		jmp end; //			skip els:
els: //					} else { run if left >= 0
		xor edx, edx; //	zero edx
end: //					} endif
		shld edx, eax, SHIFT; //Shift the high bits of eax(left) into the low
		//of edx.
		shl eax, SHIFT; //Shift out the high bits of eax(left).
		idiv right; //divide by the denominator(right).
	}
}

fixed Fixed_Point::fractionalPart(fixed f) {
	__asm {
		mov eax, f; //Move f into eax.
		cmp eax, 0; //Compare f to 0.
		jge els;
		or eax, INTEGER_PART; //Set the integer bits to 1.
		jmp end;
els:
		and eax, FRACTION_PART; //0 the integer bits.
end:
	}
}

fixed Fixed_Point::integerPart(fixed f) {
	__asm {
		mov eax, f; //Move f into eax.
		mov edx, eax; //Move f into edx.
		and eax, INTEGER_PART; //0 the fraction bits of eax.
		cmp edx, 0;
		jge end;
		and edx, FRACTION_PART; //0 the integer bits of edx.
		cmp edx, 0;
		je end;
		mov edx, 1; //Move 1 into edx.
		tzcnt ecx, eax; //Get the trailing 0s to ecx.
		cmp ecx, SHIFT;
		je no_trail;
		add ecx, -1; //Subtract 1 from the trail.
no_trail:
		shl edx, cl; //Shift the bit the number of trailing 0s.
		add eax, edx; //Add the bit to eax.
end:
	}
}

fixed Fixed_Point::itofix(__int32 i) {
	__asm {
		mov eax, i; //Move i into eax.
		sal eax, SHIFT; //Shift left.
	}
}

__int32 Fixed_Point::fixtoi(fixed f) {
	__asm {
		mov eax, f; //Move f into eax.
		mov edx, eax; //Move f into edx.
		sar eax, SHIFT; //Shift eax to the right.
		cmp edx, 0;
		jge end;
		and edx, FRACTION_PART; //0 the integer bits of edx.
		cmp edx, 0;
		je end;
		mov edx, 1; //Move 1 into edx.
		tzcnt ecx, eax; //Get trailing 0s in eax.
		cmp ecx, 0;
		je no_trail;
		add ecx, -1; //Sub 1 from ecx.
no_trail:
		shl edx, cl; //Shift the bit left by the number of 0s.
		add eax, edx; //Add the bit.
end:
	}
}

bool Fixed_Point::north_hemesphere(fixed a) {
	_asm {
		mov eax, a;
		and eax, CIRCLE_BITS;
		cmp eax, HALF_CIRCLE;
		jl els;
		mov al, 0;
		jmp end;
els:
		mov al, 1;
end:
	}
}

bool Fixed_Point::right_hemesphere(fixed a) {
	_asm {
		mov eax, a;
		add eax, QUARTER_CIRCLE;
		and eax, CIRCLE_BITS;
		cmp eax, HALF_CIRCLE;
		jl els;
		mov al, 0;
		jmp end;
els:
		mov al, 1;
end:
	}
}

__int32 Fixed_Point::quartile(fixed a) {
	_asm {
		mov ebx, a;
		mov ecx, ebx;
		//North
		and ebx, CIRCLE_BITS;
		cmp ebx, HALF_CIRCLE;
		jl elsNorth;
		mov eax, 2;
		mov ebx, 0;
		jmp right;
elsNorth:
		mov eax, 0;
		mov ebx, 1;
right:
		//Right
		add ecx, QUARTER_CIRCLE;
		and ecx, CIRCLE_BITS;
		cmp ecx, HALF_CIRCLE;
		jl elsRight;
		mov ecx, 0;
		jmp quart;
elsRight:
		mov ecx, 1;
quart:
		//Quartile
		cmp ebx, ecx;
		je end;
		add eax, 1;
end:
	}
}

__int32 Fixed_Point::revolutions(fixed a) {
	__asm {
		mov eax, a;
		sar eax, REVOLUTION_SHIFT_INT;
	}
}

__int32 Fixed_Point::fixRevolutions(fixed a) {
	_asm {
		mov eax, a;
		sar eax, REVOLUTION_SHIFT_FIX;
	}
}
