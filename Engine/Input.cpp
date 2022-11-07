#include "Input.h"
namespace Input
{

	//DirectInputオブジェクト
	LPDIRECTINPUT8   pDInput_ = nullptr;//LPから始まるものはポインタ

	//キーボード
	LPDIRECTINPUTDEVICE8 pKeyDevice_ = nullptr;//デバイスオブジェクト　キーボードの準備
	BYTE keyState[256] = { 0 };
	BYTE prevKeyState[256];    //前フレームでの各キーの状態

	//マウス
	LPDIRECTINPUTDEVICE8	pMouseDevice_;	//デバイスオブジェクト
	DIMOUSESTATE mouseState_;				//マウスの状態
	DIMOUSESTATE prevMouseState_;			//前フレームのマウスの状態				
	XMFLOAT3 mousePosition;                 //マウスカーソルの位置
 
	//コントローラー
	const int MAX_PAD_NUM = 4;
	XINPUT_STATE controllerState_[MAX_PAD_NUM];//コントローラーの状態
	XINPUT_STATE prevControllerState_[MAX_PAD_NUM];

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr;
		//DirectInput本体
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput_, nullptr);
		if (FAILED(hr))
		{
			return hr;
		}

		//キーボード
		hr = pDInput_->CreateDevice(GUID_SysKeyboard, &pKeyDevice_, nullptr);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pKeyDevice_->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pKeyDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr))
		{
			return hr;
		}

		//マウス
		hr = pDInput_->CreateDevice(GUID_SysMouse, &pMouseDevice_, nullptr);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pMouseDevice_->SetDataFormat(&c_dfDIMouse);
		if (FAILED(hr))
		{
			return hr;
		}
		hr = pMouseDevice_->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	void Update()
	{
		//キーボード
		pKeyDevice_->Acquire();
		memcpy(prevKeyState,keyState, sizeof(keyState));//第一引数にコピー先のメモリブロックのポインタ第二引数でコピー元のポインタ第三引数でコピーサイズ
		pKeyDevice_->GetDeviceState(sizeof(keyState), &keyState);//すべてのキーの情報が配列に入る

		//マウス
		pMouseDevice_->Acquire();
		memcpy(&prevMouseState_, &mouseState_, sizeof(mouseState_));//第一引数にコピー先のメモリブロックのポインタ第二引数でコピー元のポインタ第三引数でコピーサイズ
		pMouseDevice_->GetDeviceState(sizeof(mouseState_), &mouseState_);

		//コントローラー
		for (int i = 0; i < MAX_PAD_NUM; i++)
		{
			memcpy(&prevControllerState_[i], &controllerState_[i], sizeof(controllerState_[i]));//第一引数にコピー先のメモリブロックのポインタ第二引数でコピー元のポインタ第三引数でコピーサイズ
			XInputGetState(i, &controllerState_[i]);
		}
	}


	void Release()
	{
		SAFE_RELEASE(pDInput_);
		SAFE_RELEASE(pKeyDevice_);
		SAFE_RELEASE(pMouseDevice_);
	}

	/////////////////////////////　キーボード情報取得　//////////////////////////////////
	//キーが押されているか調べる
	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80)//アンドをとる　１６進表記
		{
			return true;
		}
		return false;
	}

	//キーを今押したか調べる（押しっぱなしは無効）
	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (!(prevKeyState[keyCode] & 0x80) && IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	//キーを今放したか調べる
	bool IsKeyUp(int keyCode)
	{
		//さっきは押してて、今は押してない状態
		if ((prevKeyState[keyCode] & 0x80) && !IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	

	/////////////////////////////　マウス情報取得　//////////////////////////////////

	//マウスのボタンが押されているか調べる
	bool IsMouseButton(int buttonCode)
	{
		//押してる
		if (mouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsMouseButtonDown(int buttonCode)
	{
		//今は押してて、前回は押してない
		if (IsMouseButton(buttonCode) && !(prevMouseState_.rgbButtons[buttonCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	//マウスのボタンを今放したか調べる
	bool IsMouseButtonUp(int buttonCode)
	{
		//今押してなくて、前回は押してる
		if (!IsMouseButton(buttonCode) && prevMouseState_.rgbButtons[buttonCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	//マウスカーソルの位置を取得
	XMFLOAT3 GetMousePosition()
	{
		return mousePosition;
	}

	//マウスカーソルの位置をセット
	void SetMousePosition(int x, int y)
	{
		XMVECTOR v = XMVectorSet((float)x, (float)y, 0, 0);
		XMFLOAT3 f;
		XMStoreFloat3(&f, v);

		mousePosition = f;
	}

	//そのフレームでのマウスの移動量を取得
	XMFLOAT3 GetMouseMove()
	{
		XMFLOAT3 result = XMFLOAT3((float)mouseState_.lX, (float)mouseState_.lY, (float)mouseState_.lZ);
		return result;
	}

	/////////////////////////////　コントローラー情報取得　//////////////////////////////////

	//コントローラーのボタンが押されているか調べる
	bool IsPadButton(int buttonCode, int padID)
	{
		if (controllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true; //押してる
		}
		return false; //押してない
	}

	//コントローラーのボタンを今押したか調べる（押しっぱなしは無効）
	bool IsPadButtonDown(int buttonCode, int padID)
	{
		//今は押してて、前回は押してない
		if (IsPadButton(buttonCode, padID) && !(prevControllerState_[padID].Gamepad.wButtons & buttonCode))
		{
			return true;
		}
		return false;
	}

	//コントローラーのボタンを今放したか調べる
	bool IsPadButtonUp(int buttonCode, int padID)
	{
		//今押してなくて、前回は押してる
		if (!IsPadButton(buttonCode, padID) && prevControllerState_[padID].Gamepad.wButtons & buttonCode)
		{
			return true;
		}
		return false;
	}

	float GetAnalogValue(int raw, int max, int deadZone)
	{
		float result = (float)raw;//padIDが入りそのIDのコントローラーの状態が入る

		if (result > 0)
		{
			//デッドゾーン
			if (result < deadZone)
			{
				result = 0;
			}
			else
			{
				result = (result - deadZone) / (max - deadZone);
			}
		}

		else
		{
			//デッドゾーン
			if (result > -deadZone)
			{
				result = 0;
			}
			else
			{
				result = (result + deadZone) / (max - deadZone);
			}
		}

		return result;
	}

	//左スティックの傾きを取得
	XMFLOAT3 GetPadStickL(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLX, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbLY, 32767, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//右スティックの傾きを取得
	XMFLOAT3 GetPadStickR(int padID)
	{
		float x = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRX, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		float y = GetAnalogValue(controllerState_[padID].Gamepad.sThumbRY, 32767, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
		return XMFLOAT3(x, y, 0);
	}

	//左トリガーの押し込み具合を取得
	float GetPadTrrigerL(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bLeftTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//右トリガーの押し込み具合を取得
	float GetPadTrrigerR(int padID)
	{
		return GetAnalogValue(controllerState_[padID].Gamepad.bRightTrigger, 255, XINPUT_GAMEPAD_TRIGGER_THRESHOLD);
	}

	//振動させる
	void SetPadVibration(int l, int r, int padID)
	{
		XINPUT_VIBRATION vibration;
		ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
		vibration.wLeftMotorSpeed = l; // 左モーターの強さ
		vibration.wRightMotorSpeed = r;// 右モーターの強さ
		XInputSetState(padID, &vibration);
	}
}