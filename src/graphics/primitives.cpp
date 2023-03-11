#include "primitives.h"

namespace LEAF
{
Vec2& Vec2::operator+(Vec2& other){ x += other.x;   y += other.y;   return *this;   }
Vec2& Vec2::operator-(Vec2& other){ x -= other.x;   y -= other.y;   return *this;   }
Vec2& Vec2::operator/(Vec2& other){ x /= other.x;   y /= other.y;   return *this;   }
Vec2& Vec2::operator*(Vec2& other){ x *= other.x;   y *= other.y;   return *this;   }
Vec2& Vec2::operator+(float value){ x += value;     y += value;     return *this;   }
Vec2& Vec2::operator-(float value){ x -= value;     y -= value;     return *this;   }
Vec2& Vec2::operator/(float value){ x /= value;     y /= value;     return *this;   }
Vec2& Vec2::operator*(float value){ x *= value;     y *= value;     return *this;   }

Vec3& Vec3::operator+(Vec3& other){ x += other.x;   y += other.y;   z += other.z;   return *this;   }
Vec3& Vec3::operator-(Vec3& other){ x -= other.x;   y -= other.y;   z -= other.z;   return *this;   }
Vec3& Vec3::operator/(Vec3& other){ x /= other.x;   y /= other.y;   z /= other.z;   return *this;   }
Vec3& Vec3::operator*(Vec3& other){ x *= other.x;   y *= other.y;   z *= other.z;   return *this;   }
Vec3& Vec3::operator+(float value){ x += value;     y += value;     z += value;     return *this;   }
Vec3& Vec3::operator-(float value){ x -= value;     y -= value;     z -= value;     return *this;   }
Vec3& Vec3::operator/(float value){ x /= value;     y /= value;     z /= value;     return *this;   }
Vec3& Vec3::operator*(float value){ x *= value;     y *= value;     z *= value;     return *this;   }

Vec4& Vec4::operator+(Vec4& other){ x += other.x;   y += other.y;   z += other.z;   w += other.w;   return *this;   }
Vec4& Vec4::operator-(Vec4& other){ x -= other.x;   y -= other.y;   z -= other.z;   w -= other.w;   return *this;   }
Vec4& Vec4::operator/(Vec4& other){ x /= other.x;   y /= other.y;   z /= other.z;   w /= other.w;   return *this;   }
Vec4& Vec4::operator*(Vec4& other){ x *= other.x;   y *= other.y;   z *= other.z;   w *= other.w;   return *this;   }
Vec4& Vec4::operator+(float value){ x += value;     y += value;     z += value;     w += value;     return *this;   }
Vec4& Vec4::operator-(float value){ x -= value;     y -= value;     z -= value;     w -= value;     return *this;   }
Vec4& Vec4::operator/(float value){ x /= value;     y /= value;     z /= value;     w /= value;     return *this;   }
Vec4& Vec4::operator*(float value){ x *= value;     y *= value;     z *= value;     w *= value;     return *this;   }

Vec2& Vec2::operator=(Vec2& other){ x = other.x; y = other.y; return *this;  }
bool Vec2::operator==(Vec2& other){ return x == other.x && y == other.y;  }

Vec3& Vec3::operator=(Vec3& other){ vec2 = other.vec2; z = other.z; return *this;  }
Vec3& Vec3::operator=(Vec2& other){ vec2 = other;      z = 0;       return *this;  }
bool Vec3::operator==(Vec3& other){ return x == other.x && y == other.y && z == other.z;  }

Vec4& Vec4::operator=(Vec4& other){ vec3 = other.vec3;         w = other.w; return *this;  }
Vec4& Vec4::operator=(Vec3& other){ vec3 = other;              w = 0;       return *this;  }
Vec4& Vec4::operator=(Vec2& other){ vec2 = other;       z = 0; w = 0;       return *this;  }
bool Vec4::operator==(Vec4& other){ return x == other.x && y == other.y && z == other.z && w == other.w;  }

Quad::Quad()
{
    position[0] = Vec4(-1.0f,   1.0f, 1.0f, 1.0f );
    position[1] = Vec4( 1.0f,   1.0f, 1.0f, 1.0f );
    position[2] = Vec4(-1.0f,  -1.0f, 1.0f, 1.0f );
    position[3] = Vec4( 1.0f,  -1.0f, 1.0f, 1.0f );
    
    uvs[0] = Vec2( 0.0f,   1.0f );
    uvs[1] = Vec2( 1.0f,   1.0f );
    uvs[2] = Vec2( 0.0f,   0.0f );
    uvs[3] = Vec2( 1.0f,   0.0f );
    
    indices[0] = 2;
    indices[1] = 1;
    indices[2] = 0;
    indices[3] = 2;
    indices[4] = 1;
    indices[5] = 3;
}

}