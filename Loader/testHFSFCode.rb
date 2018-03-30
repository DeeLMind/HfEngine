Program("Draw") { #定义一个叫做Draw的Program
	Code(%{       
	float4 VS(float4 pos : POSITION) : SV_POSITION {
		return pos;
	}
	cbuffer param : register(b0) {
		float4 color;
	}
	float4 PS(float4 pos : POSITION) : SV_TARGET {
		return color;
	}
	}) #HLSL代码
	InputLayout("layout") {  
		Format "POSITION", DX::R32G32B32_FLOAT 
	}
	VertexBuffer("vb1"){
		set_size 4*4*4
	}
	IndexBuffer("ib1"){
		set_size 4*4
	}
	Resource("res") { #资源段
		Sampler("color_sampler") {
			set_filter DX::FILTER_MIN_MAG_MIP_LINEAR, 0  #三线性
			set_uvwaddress DX::ADDRESS_WRAP, DX::ADDRESS_WRAP, DX::ADDRESS_WRAP, HFColorRGBA(0.0, 0.0, 0.0, 0.0)
		}
		ConstantBuffer("param") {
			set_size 16
		}
		Blender("blender") {
			enable true
			set_mask DX::COLOR_WRITE_ENABLE_ALL & ~DX::COLOR_WRITE_ENABLE_GREEN  #清除原谅色~ 注意&不要和~连起来写...
			set_color_blend DX::BLEND_FACTOR, DX::BLEND_ONE, DX::BLEND_OP_ADD
			set_blend_factor HFColorRGBA(0.5, 0.0, 0.8, 1.0)
		}
	}
	Section("preset") { #Section 设置
		set_input_layout("layout")
		set_vbuffer("vb1")
		set_ibuffer("ib1")
		set_ps_cbuffer("param")
		set_vshader("VS")
		set_pshader("PS")
	}
	Section("render") { #Section 执行渲染
		draw_index(0, 4)
	}
}