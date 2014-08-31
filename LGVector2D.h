/*
 *  Vector2D.h
 *
 *  Created by Lee Grey on 11/06/11.
 *  Copyright 2011 Lee Grey. All rights reserved.
 *
 */

#ifndef _LGVECTOR2D_
#define _LGVECTOR2D_

class LGVector2D {
    
	public :
    
	float x, y;
	LGVector2D();
	LGVector2D( float xInit, float yInit );
    
	void zero();
	void set( LGVector2D& v );
	void set( float xs, float ys );
	LGVector2D clone();
    
	void setAngle( float a );
	void setAngleDeg( float a );	
	float getAngle();
	float getAngleDeg();
	void rotateBy( float l );	
    
	void add( LGVector2D& v );
	void subtract( LGVector2D& v );
	void multiply( float s );
	void divide( float s );
    
	void normalize();
	void setLength( float l );
	float getLength();
    
	float dot( LGVector2D& v );
	float cross( LGVector2D& v );
	float distanceTo( LGVector2D v );
	LGVector2D vectorTo( LGVector2D v );	
	void lookAt( LGVector2D& v );
    void reflect( LGVector2D v );
    LGVector2D unit();
    void invert();
    
	void print( bool includeEndline = true );
    
	LGVector2D operator+ ( const LGVector2D& v ) const;
	LGVector2D operator- ( const LGVector2D& v ) const;
	LGVector2D operator* ( const float s ) const;	
	float operator* ( const LGVector2D& v ) const; //dot product
	float operator^ ( const LGVector2D& v ) const; //cross product	
	LGVector2D operator/ ( const float s ) const;
	LGVector2D operator/ ( const LGVector2D& v ) const;
	bool operator== ( const LGVector2D& v ) const;
	void operator= ( const LGVector2D& v );
	void operator+= ( const LGVector2D& v );
	void operator-= ( const LGVector2D& v );
	void operator*= ( const float s );	
	void operator/= ( const float s );
	float operator[] ( int i );
    
};

LGVector2D lgv(float x, float y);

#endif