/*
 *  LGVector2D.cpp
 *
 *  Created by Lee Grey on 11/06/11.
 *  Copyright 2011 Lee Grey. All rights reserved.
 *
 */

#include "LGVector2D.h"
#include <iostream>
#include <math.h>

LGVector2D::LGVector2D() { 
	x = 0;
	y = 0;
}

LGVector2D::LGVector2D( float xInit, float yInit ) { 
	x = xInit;
	y = yInit;
}

void LGVector2D::set( LGVector2D& v ) {
	x = v.x;
	y = v.y;
}

void LGVector2D::set( float xs, float ys ) {
	x = xs;
	y = ys;
}

LGVector2D LGVector2D::clone() {
	return LGVector2D( x, y );
}

void LGVector2D::zero() {
	x = 0;
	y = 0;	
}

void LGVector2D::add( LGVector2D& v ) {
	x += v.x;
	y += v.y;
}

void LGVector2D::subtract( LGVector2D& v ) {
	x -= v.x;
	y -= v.y;
}

void LGVector2D::multiply( float s ) {
	x *= s;
	y *= s;
}

void LGVector2D::divide( float d ) {
	x /= d;
	y /= d;	
}

void LGVector2D::normalize() {
	float length = sqrt(x*x+y*y);
    if( length == 0 ) y = 1;
	x /= length;
	y /= length;
}

float LGVector2D::getLength() {	
   return sqrt(x*x + y*y);
}

void LGVector2D::setLength( float l ) {
	float length = sqrt( x*x+y*y );
    if( length == 0 ) y = 1;
	x = (x / length) * l;
	y = (y / length) * l;
}

float LGVector2D::getAngle() {
	return atan2( y, x );
}

float LGVector2D::getAngleDeg() {
	return atan2(y,x) * 57.2957;
}

void LGVector2D::setAngle( float a ) {
	float length = sqrt(x*x+y*y);
	x = cos(a)*length;
	y = sin(a)*length;
}

void LGVector2D::setAngleDeg( float a ) {
	float length = sqrt(x*x+y*y);
	a *= 0.0174532925;
	x = cos(a)*length;
	y = sin(a)*length;
}

void LGVector2D::rotateBy( float l ) {
	float angle = getAngle();
	float length = sqrt(x*x+y*y);
	x = cos(l+angle)*length;
	y = sin(l+angle)*length;
}

float LGVector2D::dot( LGVector2D& v ) {
	return x * v.x + y * v.y;
}

float LGVector2D::cross( LGVector2D& v ) {
	return (x * v.y) - (y * v.x);
}

float LGVector2D::distanceTo( LGVector2D v ) {
	LGVector2D distanceVector = LGVector2D(v.x - x, v.y - y );
	return distanceVector.getLength();
}	

LGVector2D LGVector2D::vectorTo( LGVector2D v ) {
	return LGVector2D(v.x - x, v.y - y );
}	

void LGVector2D::lookAt( LGVector2D& v ) {
	LGVector2D *vectorToTarget = new LGVector2D( v.x - x, v.y - y  );
	setAngle( vectorToTarget->getAngle() );
}


void LGVector2D::reflect( LGVector2D v ) {
    LGVector2D unitNormal = v.unit();
    *this = *this - ( unitNormal * ( (unitNormal.dot( *this ) ) * 2 ) );
}

LGVector2D LGVector2D::unit() {
    LGVector2D unit = this->clone();
    unit.normalize();
    return unit;
}

void LGVector2D::invert() {
    x = -x;
    y = -y; 
}


//OPERATORS

LGVector2D LGVector2D::operator+ ( const LGVector2D& v ) const {
	return LGVector2D( x + v.x, y + v.y );	
}

LGVector2D LGVector2D::operator- ( const LGVector2D& v ) const {
	return LGVector2D( x - v.x, y - v.y );	
}

LGVector2D LGVector2D::operator* ( const float s ) const {
	return LGVector2D( x * s, y * s );
}

//dot product
float LGVector2D::operator* ( const LGVector2D& v ) const {
    return (x * v.x + y * v.y);
}

//cross product	
float LGVector2D::operator^ ( const LGVector2D& v ) const {
	return (x * v.y) - (y * v.x);
}

LGVector2D LGVector2D::operator/ ( const float s ) const {
	return LGVector2D( x / s, y / s );
}

LGVector2D LGVector2D::operator/ ( const LGVector2D& v ) const {
	return LGVector2D( x / v.x, y / v.y );
}

bool LGVector2D::operator== ( const LGVector2D& v ) const {
	return ( x == v.x && y == v.y );
}

void LGVector2D::operator= ( const LGVector2D& v ) {
	x = v.x;
	y = v.y;
}

void LGVector2D::operator+= ( const LGVector2D& v ) {
	x += v.x;
	y += v.y;	
}

void LGVector2D::operator-= ( const LGVector2D& v ) {
	x -= v.x;
	y -= v.y;	
}

void LGVector2D::operator*= ( const float s ) {
	x *= s;
	y *= s;
}

void LGVector2D::operator/= ( const float s ) {
	x /= s;
	y /= s;
}

float LGVector2D::operator[] ( int i ) {
	if ( i == 0 ) return x;
    else return y;
}

void LGVector2D::print( bool includeEndline ) {
	if( includeEndline ) printf( "%s%f%s%f%s", "[" , x , ",", y , "]\n" );
    else printf( "%s%f%s%f%s", "[" , x , ",", y , "]" );
}

//convenience///////////////

LGVector2D lgv(float x, float y) {
    return LGVector2D(x,y);    
}
