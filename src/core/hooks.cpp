#include "hooks.h"

#include "../../ext/minhook/MinHook.h"

void hooks::Setup() noexcept
{
	MH_Initialize();

	MH_CreateHook(
		memory::Get(interfaces::clientMode, 24),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	);

	MH_EnableHook(MH_ALL_HOOKS);
}

void hooks::Destroy() noexcept
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd *cmd) noexcept
{
	const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	if (!cmd->commandNumber) {
		return result;
	}

	if (result) {
		interfaces::engine->SetViewAngles(cmd->viewAngles);
	}

	globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());

	if (!GetAsyncKeyState(VK_XBUTTON2)) {
		return false;
	}

	if (!globals::localPlayer || globals::localPlayer->IsAlive()) {
		return false;
	}

	CVector eyePosition;
	globals::localPlayer->GetEyePosition(eyePosition);

	CVector aimPunch;
	globals::localPlayer->GetAimPunch(aimPunch);

	const CVector dst = eyePosition + CVector{ cmd->viewAngles + aimPunch }.ToVector() * 1000.f;

	CTrace trace;
	interfaces::trace->TraceRay({ eyePosition, dst }, 0x46004009, globals::localPlayer, trace);

	if (!trace.entity || !trace.entity->IsPlayer()) {
		return false;
	}

	if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam()) {
		return false;
	}

	cmd->buttons |= IN_ATTACK;

	return false;
}