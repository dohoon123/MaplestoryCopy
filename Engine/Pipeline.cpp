#include <d3d11.h>
#include <assert.h>
#include "Pipeline.h"

#if not defined _DEBUG
#define MUST(Expression) ( ( (Expression)))
#else
#define MUST(Expression) (assert(SUCCEEDED(Expression)))
#endif

namespace Rendering::Pipeline
{
	namespace {// 익명 namespace. 현 파일 안에서만 사용가능
		ID3D11Device*			Device;
		ID3D11DeviceContext*	DeviceContext;
		IDXGISwapChain*			SwapChain;//백 버퍼와 프론트 버퍼의 개념
		ID3D11RenderTargetView* RenderTargetView;

		namespace Buffer {
			ID3D11Buffer* Vertex;
			ID3D11Buffer* Constant[2];//world , camera*projection

			template<typename Data>/*업데이트 할 버퍼, 버퍼를 초기화할 cpu 데이터*/
			void Update(ID3D11Buffer* Buffer,  Data const& data) {
				//버퍼를 초기화 시켜줄 객체를 선언
				D3D11_MAPPED_SUBRESOURCE SubResource = D3D11_MAPPED_SUBRESOURCE();
				
				MUST(DeviceContext->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource));
				{
					memcpy_s(SubResource.pData, SubResource.RowPitch, data, sizeof(data));
					// 버퍼의 초기화를 시켜줄 객체에 초기화할 cpu 데이터를 복사합니다.
				}
				DeviceContext->Unmap(Buffer, 0);
			}/*업데이트가끝난 버퍼 unmap*/
		}
	}

	namespace String
	{
		void Render(HFONT const hFont, LPCSTR const string, COLORREF const color, SIZE const size, POINT const center)
		{
			IDXGISurface1* Surface = nullptr;

			MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&Surface)));
			{
				HDC hDC = HDC();

				MUST(Surface->GetDC(false, &hDC));
				{
					if (hFont != HFONT()) {
						SelectObject(hDC, hFont);
					}
					SetTextColor(hDC, color);

					RECT area
					{
						center.x - size.cx / 2,
						center.y - size.cy / 2,
						center.x + size.cx / 2,
						center.y + size.cy / 2,
					};

					DrawText(hDC, string, ~'\0', &area, 0);// 여기 다시 듣기
				}
				MUST(Surface->ReleaseDC(nullptr));
			}
			Surface->Release();

			DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
		}
	}

	namespace Texture
	{
		struct Handle final
		{
			ID3D11ShaderResourceView* ShaderResourceView = nullptr;
		};
		void Create(Handle *& handle, SIZE const size, BYTE const* const data) {
			D3D11_TEXTURE2D_DESC const Descriptor
			{
				static_cast<UINT>(size.cx),
				static_cast<UINT>(size.cy),
				1, 
				1,
				DXGI_FORMAT_B8G8R8A8_UNORM,
				1,
				0,
				D3D11_USAGE_IMMUTABLE,
				D3D11_BIND_SHADER_RESOURCE
				/*
				//TEXTURE2D의 너비
				//TEXTURE2D의 높이
				1,
				1,
				데이터를 읽을 방식(format)
				안티얼라이싱 : 멀티 샘플링 샘플 개수
				quality 사용 x
				사용용도 (gpu에서만 사용)
				//식별자
				*/
			};

			UINT constexpr BPP = 32;//32비트 이미지입니다.

			D3D11_SUBRESOURCE_DATA const SubResource{
				data,
				size.cx * BPP / 8 // 왜 X4라고? 다시 듣기
			};

			ID3D11Texture2D* Texture2D = nullptr;
			MUST(Device->CreateTexture2D(&Descriptor, &SubResource, &Texture2D));
			{MUST(Device->CreateShaderResourceView(Texture2D, nullptr, &(handle = new Handle())->ShaderResourceView)); }
			Texture2D->Release();
		}
		void Render(Handle const* const& handle, RECT const area, bool const is_flipped)
		{
			DeviceContext->PSSetShaderResources(0, 1, &handle->ShaderResourceView);
			{
				if (!is_flipped) {
					float const Coordinates[4][2]
					{
						{ static_cast<float>(area.left),		static_cast<float>(area.top)	},
						{ static_cast<float>(area.right),		static_cast<float>(area.top)	},
						{ static_cast<float>(area.left),		static_cast<float>(area.bottom)	},
						{ static_cast<float>(area.right),		static_cast<float>(area.bottom)	}
					};
					Buffer::Update(Buffer::Vertex, Coordinates);
				}
				else {
					float const Coordinates[4][2]
					{
						{ static_cast<float>(area.right),		static_cast<float>(area.top)	},
						{ static_cast<float>(area.left),		static_cast<float>(area.top)	},
						{ static_cast<float>(area.right),		static_cast<float>(area.bottom)	},
						{ static_cast<float>(area.left),		static_cast<float>(area.bottom)	}
					};
					Buffer::Update(Buffer::Vertex, Coordinates);
				}
			}
			DeviceContext->Draw(4, 0);
		}

		void Delete(Handle const* const& handle)
		{
			handle->ShaderResourceView->Release();
			delete handle;
		}
	}

	namespace Transform
	{
		template<Type type>
		void Update(matrix const& matrix)
		{
			Buffer::Update(Buffer::Constant[static_cast<UINT>(type)], matrix);
		}
		template void Update<Type::Former>(matrix const& matrix);
		template void Update<Type::Latter>(matrix const& matrix);//존재한다고 언급한다.
	}

	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParameter, LPARAM const lParameter)
	{
		switch (uMessage)
		{
		case WM_CREATE:
		{
			{
				DXGI_SWAP_CHAIN_DESC Descriptor = DXGI_SWAP_CHAIN_DESC();
				Descriptor.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
				Descriptor.SampleDesc.Count = 1;
				Descriptor.BufferCount = 1;
				Descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				Descriptor.OutputWindow = hWindow;
				Descriptor.Windowed = true;
				Descriptor.Flags = DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE;

				MUST(D3D11CreateDeviceAndSwapChain(
					nullptr,
					D3D_DRIVER_TYPE_HARDWARE,
					nullptr,
					0,
					nullptr,
					0,
					D3D11_SDK_VERSION,
					&Descriptor,
					&SwapChain,
					&Device,
					nullptr,
					&DeviceContext
				));
			}
			{
#include "Shader/Bytecode/Vertex.h"
				{
					D3D11_INPUT_ELEMENT_DESC const Descriptor[2]
					{
						{//월드 좌표에 대한 요소
							"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0 
						},
						{//이미지 좌표에 대한 요소
							"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 1
						}
					};

					ID3D11InputLayout* InputLayout = nullptr;
					MUST(Device->CreateInputLayout(Descriptor, 2, Bytecode, sizeof(Bytecode), &InputLayout));
					DeviceContext->IASetInputLayout(InputLayout);
					InputLayout->Release();
					//inputlayout에 bytecode를 이용하는 이유/ 각 bytecode마다 다른 inputlayout을 사용해야해서
				}
				{
					ID3D11VertexShader* VertexShader = nullptr;
					MUST(Device->CreateVertexShader(Bytecode, sizeof(Bytecode), nullptr, &VertexShader));
					DeviceContext->VSSetShader(VertexShader, nullptr, 0);
					VertexShader->Release();
				}
			}
			{
#include "Shader/Bytecode/Pixel.h"
				ID3D11PixelShader* PixelShader = nullptr;

				MUST(Device->CreatePixelShader(
					Bytecode, sizeof(Bytecode), nullptr, &PixelShader
					//Bytecode
					//Bytecode size
					//nullptr
					// pixelshader
				));
				DeviceContext->PSSetShader(PixelShader, nullptr, 0);

				PixelShader->Release();

			}
			{
				float const Coordinates[4][2]{
					{ -0.5f, +0.5f },//좌상
					{ +0.5f, +0.5f },//우상
					{ -0.5f, -0.5f },//좌하
					{ +0.5f, -0.5f }//우하
				};

				D3D11_BUFFER_DESC const Descriptor
				{
					sizeof(Coordinates),
					D3D11_USAGE_IMMUTABLE,//버퍼의 사용용도 (gpu에서만 사용할 수 있도록)
					D3D11_BIND_VERTEX_BUFFER,//버퍼의 식별자 (vertex 식별자를 가지고 있도록 설정)
					0//cpu 접근 권한 (cpu의 접근을 쓰는 것을 불허하도록 설정)
				};

				D3D11_SUBRESOURCE_DATA const SubResource{ Coordinates };

				ID3D11Buffer* Buffer = nullptr;

				MUST(Device->CreateBuffer(&Descriptor, &SubResource, &Buffer));

				UINT const Stride = sizeof(*Coordinates);
				UINT const Offset = 0;

				DeviceContext->IASetVertexBuffers
				(
					0, 1, &Buffer, &Stride, &Offset
					/*
					startslot : 0
					buffer num : 1
					buffers :
					stride 
					offset : 0
					*/
				);
				Buffer->Release();//만든 버퍼 해제
			}
			{
				D3D11_BUFFER_DESC const Descriptor
				{
					sizeof(float[4][2]),//좌표(x,y) * 4개//버퍼의 크기 (left, top, right, bottom의 좌표를 담을 수 있도록 설정)
					D3D11_USAGE_DYNAMIC,//버퍼의 사용용도 (cpu에서도 사용할 수 있도록)
					D3D11_BIND_VERTEX_BUFFER,//버퍼의 식별자 (vertex 식별자를 가지고 있도록 설정)
					D3D11_CPU_ACCESS_WRITE//cpu 접근 권한 (cpu의 접근을 쓰는 것을 허용하도록 설정)
				};

				MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Vertex));

				UINT const Stride = sizeof(float[2]);
				UINT const Offset = 0;

				DeviceContext->IASetVertexBuffers
				(
					1, 1, &Buffer::Vertex, &Stride, &Offset
					/*
					startslot : 1
					buffer num : 1
					buffers : &Buffer::Vertex
					stride : float 두개를 읽을 수 있도록
					offset : 0
					*/
				);
			}
			{DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); }
			{
				D3D11_BUFFER_DESC Descriptor
				{
					sizeof(float[4][4]),
					D3D11_USAGE_DYNAMIC,
					D3D11_BIND_CONSTANT_BUFFER,
					D3D11_CPU_ACCESS_WRITE
				};
				{
					//Buffer 두개를 생성하여 Buffer::Constant[0], [1] 다 넣어라
					for (UINT i = 0; i < 2; ++i) {
						MUST(Device->CreateBuffer(&Descriptor, nullptr, &Buffer::Constant[i]));
					}
					// 자원을 랜더링 파이프라인에 결합
					DeviceContext->VSSetConstantBuffers(0, 2, Buffer::Constant);
				}
				{
					D3D11_BLEND_DESC Descriptor = D3D11_BLEND_DESC();
					
					// Dest (Destination) : 픽셀쉐이더에서 추출된 색
					// Src  (Source) : 덧 씌울 색

					Descriptor.RenderTarget->BlendEnable = true;
					Descriptor.RenderTarget->SrcBlend		= D3D11_BLEND_SRC_ALPHA;
					Descriptor.RenderTarget->DestBlend		= D3D11_BLEND_INV_SRC_ALPHA;
					Descriptor.RenderTarget->BlendOp		= D3D11_BLEND_OP_ADD;
					Descriptor.RenderTarget->SrcBlendAlpha  = D3D11_BLEND_ZERO;
					Descriptor.RenderTarget->DestBlendAlpha = D3D11_BLEND_ONE;
					Descriptor.RenderTarget->BlendOpAlpha	= D3D11_BLEND_OP_ADD;
					Descriptor.RenderTarget->RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;


					ID3D11BlendState* BlendState = nullptr;
					MUST(Device->CreateBlendState(&Descriptor, &BlendState));

					DeviceContext->OMSetBlendState(BlendState, nullptr, D3D11_DEFAULT_SAMPLE_MASK);

					BlendState->Release();
				}
				
			}
			return;
		}
		case WM_APP:
		{
			MUST(SwapChain->Present(0, 0));//backbuffer to frontbuffer

			float const Color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
			
			DeviceContext->ClearRenderTargetView(RenderTargetView, Color);
			return;
		}

		case WM_DESTROY:
		{
			//constant buffer 해제
			Buffer::Constant[1]->Release();//순서 반대로.. 꼼꼼함 배워갑니다
			Buffer::Constant[0]->Release();
			Buffer::Vertex->Release();
			RenderTargetView->Release();
			SwapChain->Release();
			DeviceContext->Release();
			Device->Release();
			return;
		}
		case WM_SIZE:
		{
			{
				D3D11_VIEWPORT Viewport = D3D11_VIEWPORT();
				Viewport.Width = LOWORD(lParameter);
				Viewport.Height = HIWORD(lParameter);

				DeviceContext->RSSetViewports(1, &Viewport);
			}
			{
				if (RenderTargetView != nullptr) {
					//기존의 랜더타겟뷰를 해제한다.
					RenderTargetView->Release();

					MUST(SwapChain->ResizeBuffers(
						1, LOWORD(lParameter), HIWORD(lParameter), DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_GDI_COMPATIBLE
					));
				}
				//스왑체인 버퍼를 가져온다.
				ID3D11Texture2D* Texture2D = nullptr;
				//texture2d 형식을 스왑체인의 버퍼에서 가져온다.
				MUST(SwapChain->GetBuffer(0, IID_PPV_ARGS(&Texture2D)));
				{
					IDXGISurface1* Surface = nullptr;
					MUST(Texture2D->QueryInterface(IID_PPV_ARGS(&Surface)));
					{
						HDC hDC = HDC();
						MUST(Surface->GetDC(false, &hDC));
						{ SetBkMode(hDC, TRANSPARENT); }
						MUST(Surface->ReleaseDC(nullptr));
					}
					Surface->Release();
				}
				{
					MUST(Device->CreateRenderTargetView(Texture2D, nullptr, &RenderTargetView));
					DeviceContext->OMSetRenderTargets(1, &RenderTargetView, nullptr);
				}
				//createrendertargetview를 이용하여 랜더타겟뷰를 만든다.
				//rendertargetview를 랜더링 파이프라인에 연결
				//일시적으로 가져운 texture2d객체 해제
				Texture2D->Release();
			}
			return;
		}

		}
	}
}