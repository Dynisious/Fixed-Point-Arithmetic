#pragma once

/*
Typedef of __int32 for clarity in code.*/
typedef __int32 fixed;

namespace Fixed_Point {

	/*
	Signed multiplication(left * right) of two fixed-point values*/
	fixed mul(fixed left, fixed right);

	/*
	Signed division(left / right) of two fixed-point values*/
	fixed div(fixed left, fixed right);

	/*
	Gets the fractional part of f*/
	fixed fractionalPart(fixed f);

	/*
	Gets the integer part of f*/
	fixed integerPart(fixed f);

	/*
	Converts a signed int32 to a fixed-point value.*/
	fixed itofix(__int32 i);

	/*
	Converts a fixed-point value to a signed int32.*/
	__int32 fixtoi(fixed f);

	/*
	Returns true if the angle is within the northern hemesphere.*/
	bool north_hemesphere(fixed a);

	/*
	Returns true if the angle is within the left hemesphere.*/
	bool right_hemesphere(fixed a);

	/*
	Returns a value from 0-3 specifying which quartile of the circle the angle
	is in.*/
	__int32 quartile(fixed a);

	/*
	Returns the number of complete circles defined by the passed angle as an 32
	bit integer. The sign of these revolutions are preserved.*/
	__int32 revolutions(fixed a);

	/*
	Returns the number of complete circles defined by the passed angle as a
	fixed point number. The sign of these revolutions are preserved.*/
	__int32 fixRevolutions(fixed a);

};
