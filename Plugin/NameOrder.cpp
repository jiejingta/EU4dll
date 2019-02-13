#include "stdinc.h"
#include "byte_pattern.h"


namespace NameOrder {

	uintptr_t text_compo_func_v1283;
	uintptr_t text_compo2_func_v1283;

	/*-----------------------------------------------*/

	errno_t text_compo_func_hook(EU4Version version) {
		std::string desc = "text compo func hook";

		switch (version) {
		case v1_28_3:
			// sub esp ,8
			byte_pattern::temp_instance().find_pattern("83 EC 08 56 8B F1 89 75 EC C7 45 FC 00 00 00 00 52 C7");
			if (byte_pattern::temp_instance().has_size(2, desc)) {
				text_compo_func_v1283 = byte_pattern::temp_instance().get_first().address(-0x18);
			}
			else return EU4_ERROR1;
			
			// sub esp ,8
			byte_pattern::temp_instance().find_pattern("8B 56 10 2B C1 8B 4D 10 3B C8 0F 47 C8");
			if (byte_pattern::temp_instance().has_size(1, desc)) {
				text_compo2_func_v1283 = byte_pattern::temp_instance().get_first().address(-0x19);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_28_X:
		case v1_27_X:
		case v1_25_X:
		case v1_26_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	uintptr_t heir_name_fix_v1283_end;
	__declspec(naked) void heir_name_fix_v1283_start() {
		__asm {
			pop edx;
			push eax;
			mov ecx, esi;
			call text_compo_func_v1283;
			
			push heir_name_fix_v1283_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t heir_name_hook(EU4Version version) {
		std::string desc = "heir name order fix";

		switch (version) {
		case v1_28_3:
			// mov     ecx, [ebx+918h]
			byte_pattern::temp_instance().find_pattern("8B 8B 18 09 00 00 8D 45 9C 83 EC 18");
			if (byte_pattern::temp_instance().has_size(2, desc)) {
				// mov     edx, eax
				injector::MakeJMP(byte_pattern::temp_instance().get_first().address(0x14), heir_name_fix_v1283_start);
				// add     esp, 4
				heir_name_fix_v1283_end = byte_pattern::temp_instance().get_first().address(0x1D);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_28_X:
		case v1_27_X:
		case v1_25_X:
		case v1_26_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/


	static uintptr_t tmp;
	uintptr_t name_creation_func_fix_v1283_end;
	__declspec(naked) void name_creation_func_fix_v1283_start() {
		__asm {
			add     esp, 4;

			// ��������
			mov edx, eax;
			push esi;
			lea ecx, tmp;
			call text_compo_func_v1283;
			add esp, 4;

			// esi�����Z�b�g����B���ƂŊJ�����鏈����������K�v����
			mov byte ptr[esi], 0;
			mov dword ptr[esi + 0x10], 0;
			mov dword ptr[esi + 0x14], 0;

			push    0xFFFFFFFF;
			push    0;
			push    eax;

			mov     ecx, esi;
			mov[ebp - 0x4], 1;
			call text_compo2_func_v1283;
			lea ecx, [ebp - 0x2C];
			
			push	name_creation_func_fix_v1283_end;
			ret;
		}
	}

	/*-----------------------------------------------*/

	errno_t name_creation_func_hook(EU4Version version) {
		std::string desc = "name creation func fix";

		switch (version) {
		case v1_28_3:
			// add     esp, 4
			byte_pattern::temp_instance().find_pattern("83 C4 04 6A FF 6A 00 50 8B CE C7 45 FC 01");
			if (byte_pattern::temp_instance().has_size(2, desc)) {
				injector::MakeJMP(byte_pattern::temp_instance().get_second().address(), name_creation_func_fix_v1283_start);
				// call    sub_xxxxxx (release buffer)
				name_creation_func_fix_v1283_end = byte_pattern::temp_instance().get_second().address(0x19);
			}
			else return EU4_ERROR1;
			return NOERROR;

		case v1_28_X:
		case v1_27_X:
		case v1_25_X:
		case v1_26_X:
			/* �Ή����Ȃ� */
			return NOERROR;
		}

		return EU4_ERROR1;
	}

	/*-----------------------------------------------*/

	errno_t init(EU4Version version) {
		errno_t result = 0;

		byte_pattern::temp_instance().debug_output2("name order fix(issue-98)");

		/* */
		result |= text_compo_func_hook(version);

		/*  */
		result |= heir_name_hook(version);

		/*  */
		result |= name_creation_func_hook(version);

		return result;
	}
}