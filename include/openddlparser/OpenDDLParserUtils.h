/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2014 Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once
#ifndef OPENDDLPARSER_OPENDDLPARSERUTILS_H_INC
#define OPENDDLPARSER_OPENDDLPARSERUTILS_H_INC

#include <openddlparser/OpenDDLCommon.h>

BEGIN_ODDLPARSER_NS

template<class T>
inline
bool isComment( T *in, T *end ) {
    if( *in == '/' ) {
        if( in + 1 != end ) {
            if( *( in + 1 ) == '/' ) {
                return true;
            }
        }
    }
    return false;
}

template<class T>
inline
bool isUpperCase( T in ) {
    return ( in >= 'A' && in <= 'Z' );
}

template<class T>
inline
bool isLowerCase( T in ) {
    return ( in >= 'a' && in <= 'z' );
}

template<class T> 
inline 
bool isSpace( const T in ) {
    return ( ' ' == in || '\t' == in );
}

template<class T>
inline
bool isNewLine( const T in ) {
    return ( '\n' == in );
}

template<class T>
inline
bool isSeparator( T in ) {
    if( isSpace( in ) || ',' == in || '{' == in || '}' == in ) {
        return true;
    }
    return false;
}

static const unsigned char chartype_table[ 256 ] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0-15
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 16-31
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 32-47
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, // 48-63

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 64-79
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 80-95
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 96-111
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 112-127

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // > 127 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

template<class T>
inline
bool isNumeric( const T in ) {
    return ( in >= '0' && in <= '9' );
    //return ( chartype_table[in] );
    /*if (in >= '0' &&  in <= '9' )
    return true;

    return false;*/
}

template<class T>
inline
bool isInteger( T *in, T *end ) {
    if( in != end ) {
        if( *in == '-' ) {
            in++;
        }
    }

    bool result( false );
    while( '}' != *in && ',' != *in && !isSpace( *in ) && in != end ) {
        result = isNumeric( *in );
        if( !result ) {
            break;
        }
        in++;
    }

    return result;
}

template<class T>
inline
bool isFloat( T *in, T *end ) {
    if( in != end ) {
        if( *in == '-' ) {
            in++;
        }
    }

    // check for <1>.0f
    bool result( false );
    while( !isSpace( *in ) && in != end ) {
        if( *in == '.' ) {
            result = true;
            break;
        }
        result = isNumeric( *in );
        if( !result ) {
            return false;
        }
        in++;
    }

    // check for 1<.>0f
    if( *in == '.' ) {
        in++;
    } else {
        return false;
    }

    // check for 1.<0>f
    while( !isSpace( *in ) && in != end ) {
        result = isNumeric( *in );
        if( !result ) {
            return false;
        }
        in++;
    }

    return result;
}

template<class T>
inline
bool isCharacter( const T in ) {
    return ( in >= 'a' && in <= 'z' || in >= 'A' && in <= 'Z' );
}

template<class T>
inline
bool isStringLiteral( const T in ) {
    return ( in == '\"' );
}

template<class T>
inline
bool isHexLiteral( T *in, T *end ) {
    if( *in == '0' ) {
        if( in + 1 != end ) {
            if( *( in + 1 ) == 'x' ) {
                return true;
            }
        }
    }

    return false;
}

template<class T>
inline
bool isEndofLine( const T in ) {
    return ( '\n' == in );
}

template<class T>
inline
static T *getNextSeparator( T *in, T *end ) {
    while( !isSeparator( *in ) || in == end ) {
        in++;
    }
    return in;
}

END_ODDLPARSER_NS

#endif // OPENDDLPARSER_OPENDDLPARSERUTILS_H_INC
