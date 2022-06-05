#pragma once
#include <cmath>
#include <algorithm>
#include <string>
#include <locale>
#include <d3d11.h>

#include "../divancrt/vcruntime/vcruntime.hh"
#include "../utils/xor.hh"
#include "../utils/utils.hh"



struct FNameEntryHandle {
	uint32_t Block = 0;
	uint32_t Offset = 0;

	FNameEntryHandle(uint32_t block, uint32_t offset) : Block(block), Offset(offset) {};
	FNameEntryHandle(uint32_t id) : Block(id >> 16), Offset(id & 65535) {};
	operator uint32_t() const { return (Block << 16 | Offset); }
};

struct FNameEntry {
	uint16_t bIsWide : 1;
	uint16_t LowercaseProbeHash : 5;
	uint16_t Len : 10;
	union
	{
		char AnsiName[1024];
		wchar_t	WideName[1024];
	};


	inline char* String()
	{
		CHAR ret[1024] = { "\0" };
		vcruntime->zero_mem(ret, sizeof(ret));
		vcruntime->strncpy(ret, AnsiName, Len);
		return ret;
	}
};

struct FNamePool
{
	std::byte Lock[8];
	uint32_t CurrentBlock;
	uint32_t CurrentByteCursor;
	std::byte* Blocks[8192];

	inline FNameEntry* GetEntry(FNameEntryHandle handle) const
	{
		return reinterpret_cast<FNameEntry*>(Blocks[handle.Block] + 2 * static_cast<uint64_t>(handle.Offset));
	}
};


extern FNamePool* NamePoolData;

struct FName {
	uint32_t Index;
	uint32_t Number;

	inline const char* GetName()
	{
		auto entry = NamePoolData->GetEntry(Index);
		auto name = entry->String();
		if (Number > 0)
		{
			char leading_integer[31] = { 0 };
			vcruntime->zero_mem(leading_integer, sizeof(leading_integer));
			vcruntime->strcatA(name, "_");

			auto d = vcruntime->itoa(Number, leading_integer, 10);
			vcruntime->strcatA(name, d);
		}
		auto pos = vcruntime->rfind(name, "/");
		if (pos != std::string::npos)
		{
			name = vcruntime->substr(name, pos + 1, vcruntime->strlen(name));
		}
		return name;
	}
};





struct UObject {
	void** VFTable;
	uint32_t ObjectFlags;
	uint32_t InternalIndex;
	struct UClass* ClassPrivate;
	FName NamePrivate;
	UObject* OuterPrivate;


	const char* GetName();
	const char* GetFullName();
	bool IsA(void* cmp);
};


struct UField : UObject {
	char pad_28[0x8]; // 0x28(0x08)
};

struct UStruct : UField {
	char pad_30[0x10]; // 0x30(0x80)
	UStruct* SuperStruct; // 0x40(0x8)

};

struct UClass : UStruct {
	char pad_B0[0x180]; // 0xb0(0x180)
};

/*cheat structs*/

struct TUObjectArray {
	std::byte** Objects;
	std::byte* PreAllocatedObjects;
	uint32_t MaxElements;
	uint32_t NumElements;
	uint32_t MaxChunks;
	uint32_t NumChunks;

	inline UObject* GetObjectPtr(uint32_t id) const
	{
		if (id >= NumElements) return nullptr;
		uint64_t chunkIndex = id / 65536;
		if (chunkIndex >= NumChunks) return nullptr;
		auto chunk = Objects[chunkIndex];
		if (!chunk) return nullptr;
		uint32_t withinChunkIndex = id % 65536 * 24;
		auto item = *reinterpret_cast<UObject**>(chunk + withinChunkIndex);
		return item;
	}

	inline UObject* FindObject(const char* name) const
	{
		for (auto i = 0u; i < NumElements; i++)
		{
			auto object = GetObjectPtr(i);
			if (object)
			{
				auto fname = object->GetFullName();
				if (vcruntime->strstr(fname, name))
					return object;
			}
		}
		return nullptr;
	}
};
extern TUObjectArray* ObjObjects;


// Enum Engine.EMouseLockMode
enum class EMouseLockMode : uint8_t {
	DoNotLock = 0,
	LockOnCapture = 1,
	LockAlways = 2,
	LockInFullscreen = 3,
	EMouseLockMode_MAX = 4
};


/*ue4 sdk*/
struct Vector2 {

	float x = 0.f;
	float y = 0.f;

	// Vector2() : x(0.f), y(0.f){}  

	__forceinline Vector2 operator+(const Vector2& a) const { return { x + a.x, y + a.y }; }
	__forceinline Vector2 operator-(const Vector2& a) const { return { x - a.x, y - a.y }; }
	__forceinline Vector2 operator/(const Vector2& a) const { return { x / a.x, y / a.y }; }
	__forceinline Vector2 operator/(const float a) const { return { x / a, y / a }; }


};

class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}


	float Length2() const
	{
		return vcruntime->sqrt(x * x + y * y + z * z);
	}

	float DistTo(const Vector3& vecEnd) const
	{
		return (*this - vecEnd).Length2();
	}


	float Distance(Vector3 v)
	{
		return float(vcruntime->sqrt(vcruntime->pow(v.x - x, 2.0) + vcruntime->pow(v.y - y, 2.0) + vcruntime->pow(v.z - z, 2.0)));
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}


	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	__forceinline Vector3 operator+(const Vector3& a) const { return { x + a.x, y + a.y, z + a.z }; }
	__forceinline void operator+=(const Vector3& a) { x += a.x, y += a.y, z += a.z; };
	__forceinline Vector3 operator-(const Vector3& a) const { return { x - a.x, y - a.y, z - a.z }; }
	__forceinline void operator-=(const Vector3& a) { x -= a.x, y -= a.y, z -= a.z; }
	__forceinline Vector3 operator*(const Vector3& a) const { return { x * a.x, y * a.y, z * a.z }; }
	__forceinline void operator*=(const Vector3& a) { x *= a.x, y *= a.y, z *= a.z; }
	__forceinline Vector3 operator/(const Vector3& a) const { return { x / a.x, y / a.y, z / a.z }; }
	__forceinline void operator/=(const Vector3& a) { x /= a.x, y /= a.y, z /= a.z; }
	__forceinline Vector3 operator+(const float a) const { return { x + a, y + a, z + a }; }
	__forceinline void operator+=(const float a) { x += a, y += a, z += a; }
	__forceinline Vector3 operator-(const float a) const { return { x - a, y - a, z - a }; }
	__forceinline void operator-=(const float a) { x -= a, y -= a, z -= a; }
	__forceinline Vector3 operator*(const float a) const { return { x * a, y * a, z * a }; }
	__forceinline void operator*=(const float a) { x *= a, y *= a, z *= a; }
	__forceinline Vector3 operator/(const float a) const { return { x / a, y / a, z / a }; }
	__forceinline void operator/=(const float a) { x /= a, y /= a, z /= a; }

	__forceinline bool operator==(const Vector2 a) const { return x == a.x && y == a.y; }
	__forceinline bool operator!=(const Vector2 a) const { return x != a.x || y != a.y; }
	__forceinline bool operator==(const Vector3& a) const { return x == a.x && y == a.y && z == a.z; }
	__forceinline bool operator!=(const Vector3& a) const { return x != a.x || y != a.y || z != a.z; }
	__forceinline bool operator==(const float a) const { return x == a && y == a && z == a; }
	__forceinline bool operator!=(const float a) const { return x != a || y != a || z != a; }

	__forceinline bool operator>(const Vector2 a) const { return x > a.x && y > a.y; }
	__forceinline bool operator>=(const Vector2 a) const { return x >= a.x && y >= a.y; }
	__forceinline bool operator<(const Vector2 a) const { return x < a.x&& y < a.y; }
	__forceinline bool operator<=(const Vector2 a) const { return x <= a.x && y <= a.y; }
	__forceinline bool operator>(const Vector3& a) const { return x > a.x && y > a.y && z > a.z; }
	__forceinline bool operator>=(const Vector3& a) const { return x >= a.x && y >= a.y && z >= a.z; }
	__forceinline bool operator<(const Vector3& a) const { return x < a.x&& y < a.y&& z < a.z; }
	__forceinline bool operator<=(const Vector3& a) const { return x <= a.x && y <= a.y && z <= a.z; }
	__forceinline bool operator>(const float a) const { return x > a && y > a; }
	__forceinline bool operator>=(const float a) const { return x >= a && y >= a; }
	__forceinline bool operator<(const float a) const { return x < a&& y < a; }
	__forceinline bool operator<=(const float a) const { return x <= a && y <= a; }
};

typedef struct
{
	float x;
	float y;
	float z;
	float w;
} Vector4;

struct Plane
{
	float x;
	float y;
	float z;
	float w;
};

struct Matrix
{
	Plane x;
	Plane y;
	Plane z;
	Plane w;
};

struct FKey
{
	struct FName* KeyName;                                                   // 0x0000(0x0008) (ZeroConstructor, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPrivate)
	unsigned char                                      UnknownData_T32I[0x10];                                    // 0x0008(0x0010) MISSED OFFSET (PADDING)

};

template<class T>
struct TArray
{
	friend struct FString;

public:
	T* Data;
	int32_t Count;
	int32_t Max;

	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	int Num() const
	{
		return Count;
	};

	T& operator[](int i)
	{
		return Data[i];
	};

	const T& operator[](int i) const
	{
		return Data[i];
	};

	bool IsValidIndex(int i) const {
		return i < Num();
	};

private:

};

struct FString : public TArray<wchar_t>
{
	FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? vcruntime->wcslen(other) + 1 : 0;

		if (Count)
			Data = const_cast<wchar_t*>(other);

	};

	bool IsValid() const
	{
		return Data != nullptr;
	}

	const wchar_t* c_str() const
	{
		return Data;
	}
};


class FTextData
{
public:
	FString Text;
}; //Size: 0x0008


class FText
{
public:
	char pad_0x0000[0x28];  //0x0000
	FString TextData; //0x0000
}; //Size: 0x0008


struct FLinearColor
{
	float                                              R;                                                         // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              G;                                                         // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              B;                                                         // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)
	float                                              A;                                                         // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData, NoDestructor, HasGetValueTypeHash, NativeAccessSpecifierPublic)


	inline FLinearColor()
		: R(0), G(0), B(0), A(0)
	{ }

	inline FLinearColor(float r, float g, float b, float a)
		: R(r),
		G(g),
		B(b),
		A(a)
	{ }

};


struct alignas(16) FPlane : public Vector3
{
	float                                              W;                                                        // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
};

struct alignas(16) FQuat
{
	float                                              X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Z;                                                        // 0x0008(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              W;                                                        // 0x000C(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
};

struct FMatrix
{
	struct FPlane                                      XPlane;                                                   // 0x0000(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
	struct FPlane                                      YPlane;                                                   // 0x0010(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
	struct FPlane                                      ZPlane;                                                   // 0x0020(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
	struct FPlane                                      WPlane;                                                   // 0x0030(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)

	FMatrix operator*(const FMatrix& pM2)
	{
		FMatrix pOut;
		pOut.XPlane.x = XPlane.x * pM2.XPlane.x + XPlane.y * pM2.YPlane.x + XPlane.z * pM2.ZPlane.x + XPlane.W * pM2.WPlane.x;
		pOut.XPlane.y = XPlane.x * pM2.XPlane.y + XPlane.y * pM2.YPlane.y + XPlane.z * pM2.ZPlane.y + XPlane.W * pM2.WPlane.y;
		pOut.XPlane.z = XPlane.x * pM2.XPlane.z + XPlane.y * pM2.YPlane.z + XPlane.z * pM2.ZPlane.z + XPlane.W * pM2.WPlane.z;
		pOut.XPlane.W = XPlane.x * pM2.XPlane.W + XPlane.y * pM2.YPlane.W + XPlane.z * pM2.ZPlane.W + XPlane.W * pM2.WPlane.W;
		pOut.YPlane.x = YPlane.x * pM2.XPlane.x + YPlane.y * pM2.YPlane.x + YPlane.z * pM2.ZPlane.x + YPlane.W * pM2.WPlane.x;
		pOut.YPlane.y = YPlane.x * pM2.XPlane.y + YPlane.y * pM2.YPlane.y + YPlane.z * pM2.ZPlane.y + YPlane.W * pM2.WPlane.y;
		pOut.YPlane.z = YPlane.x * pM2.XPlane.z + YPlane.y * pM2.YPlane.z + YPlane.z * pM2.ZPlane.z + YPlane.W * pM2.WPlane.z;
		pOut.YPlane.W = YPlane.x * pM2.XPlane.W + YPlane.y * pM2.YPlane.W + YPlane.z * pM2.ZPlane.W + YPlane.W * pM2.WPlane.W;
		pOut.ZPlane.x = ZPlane.x * pM2.XPlane.x + ZPlane.y * pM2.YPlane.x + ZPlane.z * pM2.ZPlane.x + ZPlane.W * pM2.WPlane.x;
		pOut.ZPlane.y = ZPlane.x * pM2.XPlane.y + ZPlane.y * pM2.YPlane.y + ZPlane.z * pM2.ZPlane.y + ZPlane.W * pM2.WPlane.y;
		pOut.ZPlane.z = ZPlane.x * pM2.XPlane.z + ZPlane.y * pM2.YPlane.z + ZPlane.z * pM2.ZPlane.z + ZPlane.W * pM2.WPlane.z;
		pOut.ZPlane.W = ZPlane.x * pM2.XPlane.W + ZPlane.y * pM2.YPlane.W + ZPlane.z * pM2.ZPlane.W + ZPlane.W * pM2.WPlane.W;
		pOut.WPlane.x = WPlane.x * pM2.XPlane.x + WPlane.y * pM2.YPlane.x + WPlane.z * pM2.ZPlane.x + WPlane.W * pM2.WPlane.x;
		pOut.WPlane.y = WPlane.x * pM2.XPlane.y + WPlane.y * pM2.YPlane.y + WPlane.z * pM2.ZPlane.y + WPlane.W * pM2.WPlane.y;
		pOut.WPlane.z = WPlane.x * pM2.XPlane.z + WPlane.y * pM2.YPlane.z + WPlane.z * pM2.ZPlane.z + WPlane.W * pM2.WPlane.z;
		pOut.WPlane.W = WPlane.x * pM2.XPlane.W + WPlane.y * pM2.YPlane.W + WPlane.z * pM2.ZPlane.W + WPlane.W * pM2.WPlane.W;
		return pOut;
	}
};

struct alignas(16) FTransform
{
	struct FQuat                                       Rotation;                                                 // 0x0000(0x0010) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
	struct Vector3                                     Translation;                                              // 0x0010(0x000C) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
	unsigned char                                      UnknownData00[0x4];                                       // 0x001C(0x0004) MISSED OFFSET
	struct Vector3                                     Scale3D;                                                  // 0x0020(0x000C) (Edit, BlueprintVisible, SaveGame, IsPlainOldData)
	unsigned char                                      UnknownData01[0x4];                                       // 0x002C(0x0004) MISSED OFFSET

	FMatrix ToMatrixWithScale()
	{
		FMatrix m;
		m.WPlane.x = Translation.x;
		m.WPlane.y = Translation.y;
		m.WPlane.z = Translation.z;

		float x2 = Rotation.X + Rotation.X;
		float y2 = Rotation.Y + Rotation.Y;
		float z2 = Rotation.Z + Rotation.Z;

		float xx2 = Rotation.X * x2;
		float yy2 = Rotation.Y * y2;
		float zz2 = Rotation.Z * z2;
		m.XPlane.x = (1.0f - (yy2 + zz2)) * Scale3D.x;
		m.YPlane.y = (1.0f - (xx2 + zz2)) * Scale3D.y;
		m.ZPlane.z = (1.0f - (xx2 + yy2)) * Scale3D.z;

		float yz2 = Rotation.Y * z2;
		float wx2 = Rotation.W * x2;
		m.ZPlane.y = (yz2 - wx2) * Scale3D.z;
		m.YPlane.z = (yz2 + wx2) * Scale3D.y;

		float xy2 = Rotation.X * y2;
		float wz2 = Rotation.W * z2;
		m.YPlane.x = (xy2 - wz2) * Scale3D.y;
		m.XPlane.y = (xy2 + wz2) * Scale3D.x;

		float xz2 = Rotation.X * z2;
		float wy2 = Rotation.W * y2;
		m.ZPlane.x = (xz2 + wy2) * Scale3D.z;
		m.XPlane.z = (xz2 - wy2) * Scale3D.x;

		m.XPlane.W = 0.0f;
		m.YPlane.W = 0.0f;
		m.ZPlane.W = 0.0f;
		m.WPlane.W = 1.0f;

		return m;
	}
};

inline const char* UObject::GetName()
{
	return NamePrivate.GetName();
}

inline const char* UObject::GetFullName()
{
	char name[ 8192 ] = {};
	vcruntime->zero_mem( name, sizeof( name ) );
	if ( ClassPrivate != nullptr )
	{
		char temp[ 8192 ] = {};
		vcruntime->zero_mem( temp, sizeof( temp ) );
		for ( auto p = OuterPrivate; p; p = p->OuterPrivate )
		{
			char stemp[ 8192 ] = {};
			vcruntime->zero_mem( stemp, sizeof( stemp ) );

			vcruntime->strcpy( stemp, p->GetName() );
			vcruntime->strcatA( stemp, "." );
			vcruntime->strcpy( temp, stemp );
		}

		vcruntime->strcpy( name, ClassPrivate->GetName() );
		vcruntime->strcatA( name, " " );
		vcruntime->strcatA( name, temp );
		vcruntime->strcatA( name, GetName() );
	}
	return name;
}

inline bool UObject::IsA( void* cmp )
{
	for ( auto super = ClassPrivate; super; super = static_cast< UClass* >( super->SuperStruct ) ) { if ( super == cmp ) { return true; } }
	return false;
}

struct UWorld {
	char pad000[ 0x30 ];
	struct ULevel* level;
	char pad038[ 0x138 ];
	struct UGameInstance* gameinstance;
};

struct ULevel {
	char pad000[ 0x98 ];
	TArray< struct AActor* > actors;
};

struct AActor {
	char pad000[ 0x148 ];
	struct APawn* instigator;
	char pad150[ 0x10 ];
	struct USceneComponent* rootcomponent;

};

struct UGameInstance {
	char pad000[ 0x38 ];
	TArray< struct ULocalPlayer* > localplayer;
};

struct ULocalPlayer {
	char pad000[ 0x30 ];
	struct APlayerController* playercontroller;
	char pad038[ 0x38 ];
	struct UGameViewportClient* viewport_client;
};

struct APlayerController {
	char pad000[ 0x288 ];
	struct ACharacter* character;
	char pad290[ 0x20 ];
	Vector3 ControlRotation;
	char pad2b8[ 0xC ];
	struct APawn* acknowledgedpawn;
};

struct USceneComponent 
{
	char pad000[ 0x144 ];
	Vector3 RelativeLocation;

};

struct APawn
{
	char pad000[ 0x160 ];
	struct USceneComponent* rootcomponent;
	char pad168[ 0x100 ];
	struct APlayerState* playerstate;
	char pad270[ 0x10 ];
	struct APlayerController* controller;
	char pad288[ 0x20 ];
	struct USkeletalMeshComponent* Mesh;
};

struct APlayerState 	// struct ASQPlayerState
{
	char pad258[ 0x420 ];
	int32_t teamId;
	char pad428[ 0x308 ];
	struct ASQSoldier* Soldier;
};

struct ASQSoldier
{
	char pad000[ 0x6b8 ];
	struct USkeletalMeshComponent* Mesh1PShadow;
	char pad6c8[ 0x50 ];
	struct USQSoldierMovement* SoldierMovement;
	char pad760[ 0x1500 ];
	float Health;
};

struct USQSoldierMovement
{
	char pad000[ 0x758 ];
	float SprintSpeedMultiplier;
};

struct ACharacter
{
	char pad000[ 0x2a8 ];
	struct USkeletalMeshComponent* Mesh;
};

struct USkeletalMeshComponent 
{
	char pad000[ 0x8 ];
};

//using worldtoscreen = bool (*) (APlayerController* actor, Vector3 world_pos, Vector2* screen_pos);
//using lineofsightto = bool(*)(APlayerController*, AActor*, Vector3, bool);
//using GetBoneMatrix = FMatrix*( * )( USkeletalMeshComponent*, FMatrix*, int );

static TUObjectArray* ObjObjects = nullptr;
static FNamePool* NamePoolData = nullptr;

//inline worldtoscreen w2s{};
//inline lineofsightto line_of_sight{};
//inline GetBoneMatrix get_bone_matrix{};

class FD3D11Viewport
{
public:
	// 0x0024
	class FD3D11DynamicRHI* GetD3DRHI() const
	{
		static std::uint8_t displacement = 0;

		if ( !displacement )
		{
			const auto address = reinterpret_cast< uintptr_t >( utils->find_pattern( _( "48 8B 4B ? E8 ? ? ? ? 48 8B 43 ? 48 8B" ) ) );
			//printf_s( "GetD3DRHI: 0x%p\n", address );

			if ( !address )
			{
				//TRACE( "%s: memory::Scan( ... ) error!", FN );
				return nullptr;
			}

			displacement = *( std::uint8_t* ) ( address + 3 );

			if ( !displacement )
			{
				//TRACE( "%s: memory::Read( ... ) error!", FN );
				return nullptr;
			}
		}

		return *( class FD3D11DynamicRHI** ) ( std::uintptr_t( this ) + displacement );
	}

	// 0x0060
	IDXGISwapChain* GetSwapChain() const
	{
		static std::uint8_t displacement = 0;

		if ( !displacement )
		{
			const auto address = reinterpret_cast< uintptr_t >( utils->find_pattern( _( "48 8B 4B ? 48 8D 54 24 ? 89" ) ) );
			//printf_s( "FD3D11Viewport::GetSwapChain: 0x%p\n", address );

			if ( !address )
			{
				//TRACE( "%s: memory::Scan( ... ) error!", FN );
				return nullptr;
			}

			displacement = *( std::uint8_t* ) ( address + 3 );

			if ( !displacement )
			{
				//TRACE( "%s: memory::Read( ... ) error!", FN );
				return nullptr;
			}
		}

		return  *( IDXGISwapChain** ) ( std::uintptr_t( this ) + displacement );
	}
};

class FD3D11DynamicRHI
{
public:

	TArray< class FD3D11Viewport* > GetViewports() const
	{
		static std::uint32_t displacement = 0;

		if ( !displacement )
		{
			const auto address = reinterpret_cast< uintptr_t >( utils->find_pattern( _( "48 81 C7 ? ? ? ? 48 8D 44 24 ? 48 89 45 90" ) ) );

			if ( !address )
			{
				//TRACE( "%s: memory::Scan( ... ) error!", FN );
				return { };
			}

			displacement = *( std::uint32_t* ) ( address + 3 );

			if ( !displacement )
			{
				//TRACE( "%s: memory::Read( ... ) error!", FN );
				return { };
			}
		}

		return *( TArray< class FD3D11Viewport* >* )( std::uintptr_t( this ) + 0x26C0 );
	}

	IDXGISwapChain* GetSwapChain() const
	{
		const auto& viewports = GetViewports();

		const auto& viewport = viewports[ 0 ];

		if ( viewport != nullptr )
		{
			return viewport->GetSwapChain();
		}
		return nullptr;
	}
};

inline FD3D11DynamicRHI* GetDynamicRHI()
{
	const auto address = reinterpret_cast< uintptr_t >( utils->find_pattern( _( "48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 8B 43" ) ) );
	//printf_s( "GetDynamicRHI: 0x%p\n", address );

	if ( !address )
	{
		return nullptr;
	}

	const auto dynamic_rhi = utils->absolute_address( address, 3, 7 );
	//printf_s( "GetDynamicRHI absolute: 0x%p\n", dynamic_rhi );

	if ( !dynamic_rhi )
	{
		//TRACE( "%s: memory::GetAbsoluteAddress( ... ) error!", FN );
		return nullptr;
	}

	return *( FD3D11DynamicRHI** ) ( dynamic_rhi );
}

enum bones
{
	HEAD = 13,
	UPPER_NECK = 7,
	NECK = 6,
	RIGHT_SHOULDER = 66,
	RIGHT_ELBOW = 89,
	RIGHT_HAND = 124,
	LEFT_SHOULDER = 93,
	LEFT_ELBOW = 94,
	LEFT_HAND = 123,
	PELVIS = 2,
	STOMACH = 4,
	CHEST = 5,
	RIGHT_THIGH = 130,
	RIGHT_KNEE = 131,
	RIGHT_FOOT = 140,
	LEFT_THIGH = 125,
	LEFT_KNEE = 126,
	LEFT_FOOT = 139,
	ROOT = 199,
};