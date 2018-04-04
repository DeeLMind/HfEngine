#pragma once

#include "stdafx.h"
#include "DX.h"
#include "D3DBuffer.h"
#include "Shaders.h"
#include "Texture2D.h"

class RenderPipeline : public Utility::ReferredObject {
public:
    ComPtr<ID3D11DeviceContext> native_context;

    //drawing renderer pipeline
    ComPtr<ID3D11InputLayout> native_input_layout;
    Utility::ReferPtr<VertexBuffer> vbuffer;
    //vertex-shader:
    Utility::ReferPtr<VertexShader> vshader;
    
    //pixel-shader:
    Utility::ReferPtr<PixelShader> pshader;
    void Initialize(D3DDevice *device) {
        device->native_device->CreateDeferredContext(0, &native_context);
    }
    void SetInputLayout(D3DDevice *device, const std::string *idents, const DXGI_FORMAT *formats, int count);
    void SetInputLayout(D3DDevice *device, const std::initializer_list<std::string> &idents,
        const std::initializer_list<DXGI_FORMAT> &formats) {
        int len1 = idents.end() - idents.begin();
        int len2 = formats.end() - formats.begin();
        if(len1 != len2)
            throw std::runtime_error("RenderPipeline::SetInputLayout: idents and formats should be in the same length");
        SetInputLayout(device, idents.begin(), formats.begin(), len1);
    }
    void SetVertexBuffer(VertexBuffer *vb);

    //VS
    void SetVertexShader(VertexShader *vs);
    void SetVSSampler(int slot, Sampler *sampler);
    void SetVSCBuffer(int slot, ConstantBuffer *cbuffer);
    void SetVSResource(int slot, Texture2D *tex);
    //PS
    void SetPixelShader(PixelShader *ps);
    void SetPSSampler(int slot, Sampler *sampler);
    void SetPSCBuffer(int slot, ConstantBuffer *cbuffer);
    void SetPSResource(int slot, Texture2D *tex);
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY topo) {
        native_context->IASetPrimitiveTopology(topo);
    }

    void Draw(int start_pos, int count) {
        native_context->Draw(count, start_pos);
    }

    void UnInitialize() {
        native_context.ReleaseAndGetAddressOf();
        native_input_layout.ReleaseAndGetAddressOf();
        vbuffer.Release();
        vshader.Release();
        pshader.Release();
    }
    virtual void Release() {
        UnInitialize();
    }
    
};

namespace Ext {
    namespace DX {
        namespace RenderPipeline {
            extern VALUE klass;

            void Init();
        }
    }
}