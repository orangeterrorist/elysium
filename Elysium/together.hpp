#pragma once

#include "il2cpp.hpp"

namespace together 
{
	namespace UnityEngine
	{
		struct Object : Il2CppObject {
			void* m_CatchedPtr;
		};

		struct Transform;
		struct GameObject;
		struct Component : Object {
			typedef Transform*(*_tget_transform)(Component* __this, const MethodInfo* method);
			inline static _tget_transform get_transform;

			typedef GameObject*(*_tget_gameObject)(Component* __this, const MethodInfo* method);
			inline static _tget_gameObject get_gameObject;
		};

		struct Vector3 {
			float x;
			float y;
			float z;
		};

		struct Quarternion {
			float x;
			float y;
			float z;
			float w;

			typedef Vector3(*_tget_eulerAngles)(Il2CppObject __this, const MethodInfo* method);
			inline static _tget_eulerAngles get_eulerAngles;

			typedef void(*_tset_eulerAngles)(Il2CppObject __this, Vector3 value, const MethodInfo* method);
			inline static _tset_eulerAngles set_eulerAngles;
		};

		struct Transform : Component {
			typedef Vector3(*_tget_position)(Transform* __this, const MethodInfo* method);
			inline static _tget_position get_position;

			typedef void(*_tset_localScale)(Transform* __this, Vector3 value, const MethodInfo* method);
			inline static _tset_localScale set_localScale;

			typedef void(*_tset_position)(Transform* __this, Vector3 value, const MethodInfo* method);
			inline static _tset_position set_position;

			typedef Vector3(*_tget_forward)(Transform* __this, const MethodInfo* method);
			inline static _tget_forward get_forward;

			typedef Quarternion(*_tget_rotation)(Transform* __this, const MethodInfo* method);
			inline static _tget_rotation get_rotation;

			typedef Transform* (*_tGetChild)(Transform* __this, int index, const MethodInfo* method);
			inline static _tGetChild GetChild;

			typedef void(*_tSetParent)(Transform* __this, Transform* parent, bool worldPositionStays, const MethodInfo* method);
			inline static _tSetParent SetParent;
		};
	}

	namespace RecRoom
	{
		namespace Core
		{
			namespace Encounters
			{
				struct BaseSpawnEnemyEncounterStep {
					char __pad1[0x68];
					int livingEnemies;

					typedef void(*_tMasterKillAllLivingEnemies)(BaseSpawnEnemyEncounterStep* __this, const MethodInfo* method);
					inline static _tMasterKillAllLivingEnemies MasterKillAllLivingEnemies;
				};
			}
		}
	}
}