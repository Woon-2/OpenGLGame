#ifndef _glbase
#define _glbase

#include "glincludes.h"
#include <vector>
#include <array>
#include <sstream>
#include "texture.h"
#include "ordinal.h"
#include "fileread.h"
#include <numeric>
#include "CClass.h"
#include "indent.h"
#include "TMP.h"

class VAO
{
public:
	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "validity: " << is_valid << '\n';
			ss << fore_indent << indent << "VAO: " << vao << '\n';
		}

		return ss;
	}

	VAO() : is_valid{ true }
	{
		glGenVertexArrays( 1, &vao );
	}

	~VAO()
	{
		if ( is_valid )
		{
			glDeleteVertexArrays( 1, &vao );
		}
	}

	VAO( const VAO& other ) = default;

	VAO& operator=( const VAO& other ) = default;

	VAO( VAO&& other ) noexcept : vao{ other.vao }, is_valid{ other.is_valid }
	{
		other.is_valid = false;
	}

	VAO& operator=( VAO&& other ) noexcept
	{
		if ( this != &other )
		{
			vao = other.vao;
			is_valid = other.is_valid;
			other.is_valid = false;
		}

		return *this;
	}

	void bind_this() const
	{
		glBindVertexArray( vao );
	}

	void draw_array( const GLenum mode, const size_t first, const size_t cnt ) const
	{
		bind_this();
		glDrawArrays( mode, first, cnt );
	}

	void draw_elements( const GLenum mode, const size_t cnt, const GLenum type, void* indices ) const
	{
		bind_this();
		glDrawElements( mode, cnt, type, indices );
	}

private:
	GLuint vao;
	bool is_valid;
};

bool VAO::will_get_desc = true;

class BO_base
{
public:
	static constexpr GLenum static_draw = GL_STATIC_DRAW;
	static constexpr GLenum stream_draw = GL_STREAM_DRAW;
	static constexpr GLenum dynamic_draw = GL_DYNAMIC_DRAW;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "Buffer: " << buf << '\n';
			ss << fore_indent << indent << "Target: " << get_enumtype() << '\n';
		}

		return ss;
	}

	BO_base()
	{
		glGenBuffers( 1, &buf );
	}

	~BO_base()
	{
		glDeleteBuffers( 1, &buf );
	}

	void init( const GLenum target, const size_t size, void* data, const GLenum draw_option )
	{
		this->target = target;
		glBindBuffer( target, buf );
		glBufferData( target, size, data, draw_option );
	}

	void setVA( const size_t index, const size_t size, const GLenum type, const bool normalized,
		const size_t stride, const size_t offset )
	{
		glBindBuffer( target, buf );
		glVertexAttribPointer( index, size, type, normalized, stride, reinterpret_cast<void*>( offset ) );
		glEnableVertexAttribArray( index );
	}

protected:
	GLuint buf;
	GLenum target;

private:
	const std::string get_enumtype() const
	{
		switch ( target )
		{
		default:
			return "Error Type";

		case GL_ARRAY_BUFFER:
			return "GL_ARRAY_BUFFER";

		case GL_ELEMENT_ARRAY_BUFFER:
			return "GL_ELEMENT_ARRAY_BUFFER";
		}
	}
};

bool BO_base::will_get_desc = true;

class VBO : public BO_base
{
public:
	void init( const size_t size, void* data, const GLenum draw_option )
	{
		BO_base::init( GL_ARRAY_BUFFER, size, data, draw_option );
	}
};

class EBO : public BO_base
{
public:
	void init( const size_t size, void* data, const GLenum draw_option )
	{
		BO_base::init( GL_ELEMENT_ARRAY_BUFFER, size, data, draw_option );
	}
};

class ComponentVertex
{
public:
	using Elem_t = float;
	using Idx_t = size_t;
	static constexpr size_t POS_VSIZE = 3u;
	static constexpr size_t COLOR_VSIZE = 3u;
	static constexpr size_t NORMAL_VSIZE = 3u;
	static constexpr size_t TEXTURE_VSIZE = 2u;

	using Pos_t = glm::vec< POS_VSIZE, Elem_t >;
	using Normal_t = glm::vec< NORMAL_VSIZE, Elem_t >;
	using Color_t = glm::vec< COLOR_VSIZE, Elem_t >;
	using Texture_t = glm::vec< TEXTURE_VSIZE, Elem_t >;

	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "" ) const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << fore_indent << indent << "Shader Program ID: " << shader_program_id << '\n';
			ss << vao.get_desc( fore_indent + indent ).rdbuf();
			for ( const auto& vbo : vbos )
			{
				ss << vbo.get_desc( fore_indent + indent ).rdbuf();
			}
			ss << ebo.get_desc( fore_indent + indent ).rdbuf();
			ss << fore_indent << indent << "AttPos Size: " << att_pos.size() << '\n';
			ss << fore_indent << indent << "AttColor Size: " << att_color.size() << '\n';
			ss << fore_indent << indent << "AttNormal Size: " << att_normal.size() << '\n';
			ss << fore_indent << indent << "AttTexture Size: " << att_texture.size() << '\n';
			ss << fore_indent << indent << "AttIndex Size: " << att_index.size() << '\n';
		}

		return ss;
	}

	const size_t size() const
	{
		return att_index.size();
	}

	template < typename ... Args >
	void append_pos( Args&& ... args )
	{
		int dummy[ sizeof...( Args ) ] = { ( att_pos.push_back( std::forward< Args >( args ) ), 0 )... };
	}

	template < typename ... Args >
	void append_color( Args&& ... args )
	{
		int dummy[ sizeof...( Args ) ] = { ( att_color.push_back( std::forward< Args >( args ) ), 0 )... };
	}

	template < typename ... Args >
	void append_normal( Args&& ... args )
	{
		int dummy[ sizeof...( Args ) ] = { ( att_normal.push_back( std::forward< Args >( args ) ), 0 )... };
	}

	template < typename ... Args >
	void append_texture( Args&& ... args )
	{
		int dummy[ sizeof...( Args ) ] = { ( att_texture.push_back( std::forward< Args >( args ) ), 0 )... };
	}

	template < typename ... Idxs >
	void append_index( Idxs ... args )
	{
		int dummy[ sizeof...( Idxs ) ] = { ( att_index.push_back( args ), 0 )... };
	}

	void clear_pos() { att_pos.clear(); }
	void clear_color() { att_color.clear(); }
	void clear_normal() { att_normal.clear(); }
	void clear_texture() { att_texture.clear(); }

	void set_shader_program( GLuint shader_program_id ) { this->shader_program_id = shader_program_id; }

	void init( GLuint pos_location, GLuint color_location, GLuint normal_location, GLuint texture_location )
	{
		glUseProgram( shader_program_id );
		vao.bind_this();
		init_attpos( pos_location );
		init_attcolor( color_location );
		init_attnormal( normal_location );
		init_atttexture( texture_location );
		init_attindex();
	}

	void init_pos( GLuint pos_location )
	{
		glUseProgram( shader_program_id );
		vao.bind_this();
		init_attpos( pos_location );
	}

	void init_color( GLuint color_location )
	{
		glUseProgram( shader_program_id );
		vao.bind_this();
		init_attcolor( color_location );
	}

	void init_normal( GLuint normal_location )
	{
		glUseProgram( shader_program_id );
		vao.bind_this();
		init_attnormal( normal_location );
	}

	void init_texture( GLuint texture_location )
	{
		glUseProgram( shader_program_id );
		vao.bind_this();
		init_atttexture( texture_location );
	}

	void init_index()
	{
		glUseProgram( shader_program_id );
		vao.bind_this();
		init_attindex();
	}

	void load_obj( const char* file_path, const size_t index_per_face )
	{
		auto source = read_file( file_path );
		auto cnts = load_obj_get_cnts( source );
		load_obj_interpret( source, index_per_face, cnts );
	}

	ComponentVertex() = default;

	ComponentVertex( GLuint shader_program_id ) : shader_program_id{ shader_program_id } {}

	ComponentVertex( const ComponentVertex& other ) : att_pos{ other.att_pos }, att_color{ other.att_color },
		att_normal{ other.att_normal }, att_texture{ other.att_texture }, shader_program_id{ other.shader_program_id },
		att_index{ other.att_index } {}

	ComponentVertex& operator=( const ComponentVertex& other )
	{
		if ( this != &other )
		{
			att_pos = other.att_pos;
			att_color = other.att_color;
			att_normal = other.att_normal;
			att_texture = other.att_texture;
			att_index = other.att_index;
			shader_program_id = other.shader_program_id;
		}

		return *this;
	}

	ComponentVertex( ComponentVertex&& ) = default;
	ComponentVertex& operator=( ComponentVertex&& ) = default;

	friend class ComponentRender;

private:
	VAO vao;
	enum class VBO_label { POS = 0, COLOR, NORMAL, TEXTURE };
	std::array< VBO, 4 > vbos;
	EBO ebo;

	std::vector< Pos_t > att_pos;
	std::vector< Color_t > att_color;
	std::vector< Normal_t > att_normal;
	std::vector< Texture_t > att_texture;
	std::vector< Idx_t > att_index;
	GLuint shader_program_id;

	void init_attpos( GLuint pos_location )
	{
		if ( att_pos.size() )
		{
			vbos[ etoi( VBO_label::POS ) ].init( att_pos.size() * sizeof( Elem_t ) * POS_VSIZE, att_pos.data(), VBO::static_draw );
			vbos[ etoi( VBO_label::POS ) ].setVA( pos_location, POS_VSIZE, get_enumtype(), false, POS_VSIZE * sizeof( Elem_t ), 0u );
		}
	}

	void init_attcolor( GLuint color_location )
	{
		if ( att_color.size() )
		{
			vbos[ etoi( VBO_label::COLOR ) ].init( att_color.size() * sizeof( Elem_t ) * COLOR_VSIZE, att_color.data(), VBO::static_draw );
			vbos[ etoi( VBO_label::COLOR ) ].setVA( color_location, COLOR_VSIZE, get_enumtype(), false, COLOR_VSIZE * sizeof( Elem_t ), 0u );
		}
	}

	void init_attnormal( GLuint normal_location )
	{
		if ( att_normal.size() )
		{
			vbos[ etoi( VBO_label::NORMAL ) ].init( att_normal.size() * sizeof( Elem_t ) * NORMAL_VSIZE, att_normal.data(), VBO::static_draw );
			vbos[ etoi( VBO_label::NORMAL ) ].setVA( normal_location, NORMAL_VSIZE, get_enumtype(), false, NORMAL_VSIZE * sizeof( Elem_t ), 0u );
		}
	}

	void init_atttexture( GLuint texture_location )
	{
		if ( att_texture.size() )
		{
			vbos[ etoi( VBO_label::TEXTURE ) ].init( att_texture.size() * sizeof( Elem_t ) * TEXTURE_VSIZE, att_texture.data(), VBO::static_draw );
			vbos[ etoi( VBO_label::TEXTURE ) ].setVA( texture_location, TEXTURE_VSIZE, get_enumtype(), false, TEXTURE_VSIZE * sizeof( Elem_t ), 0u );
		}
	}

	void init_attindex()
	{
		if ( att_index.size() ) ebo.init( att_index.size() * sizeof( Idx_t ), att_index.data(), EBO::static_draw );
	}

	constexpr const GLenum get_enumtype() const
	{
		if constexpr ( std::is_same_v< Elem_t, float > )
		{
			return GL_FLOAT;
		}
		else
		{
			return 0;
		}
	}

	struct _Obj_Cnts
	{
		size_t v_cnt;
		size_t vn_cnt;
		size_t vt_cnt;
		size_t i_cnt;

		_Obj_Cnts() : v_cnt{ 0 }, vn_cnt{ 0 }, vt_cnt{ 0 }, i_cnt{ 0 } {}
	};

	_Obj_Cnts load_obj_get_cnts( const std::string& source )
	{
		_Obj_Cnts ret;
		std::stringstream ss{ source };

		while ( ss.good() )
		{
			char c = ss.get();

			switch ( c )
			{
			default:
				break;

			case '\n': case ' ': case EOF:
				break;

			case '#':
				while ( ss.get() != '\n' )
					;
				break;

			case 'v':
				if ( ss.peek() == 'n' )
				{
					while ( ss.get() != '\n' )
						;
					++ret.vn_cnt;
				}
				else if ( ss.peek() == 't' )
				{
					while ( ss.get() != '\n' )
						;
					++ret.vn_cnt;
				}
				else // v
				{
					while ( ss.get() != '\n' )
						;
					++ret.v_cnt;
				}
				break;

			case 'f':
				while ( ss.get() != '\n' )
					;
				ret.i_cnt += 3;
				break;
			}
		}

		return ret;
	}

	void load_obj_interpret( const std::string& source, const size_t index_per_face, const _Obj_Cnts& cnts )
	{
		std::stringstream ss{ source };

		std::vector< std::vector< Normal_t > > normals_per_vertex;
		std::vector< std::vector< Texture_t > > textures_per_vertex;
		std::vector< Normal_t > normal_per_face;
		std::vector< Texture_t > texture_per_face;

		att_pos.reserve( cnts.v_cnt );
		att_index.reserve( cnts.i_cnt );
		att_normal.reserve( cnts.v_cnt );
		att_texture.reserve( cnts.v_cnt );
		att_color.reserve( cnts.v_cnt );
		normals_per_vertex.reserve( cnts.v_cnt );
		textures_per_vertex.reserve( cnts.v_cnt );
		normal_per_face.reserve( cnts.vn_cnt );
		texture_per_face.reserve( cnts.vt_cnt );

		while ( ss.good() )
		{
			char c = ss.get();

			switch ( c )
			{
			default:
				std::cout << "obj 파일 해석 불능\n";
				break;

			case '\n': case ' ': case EOF:
				break;

			case '#':
				while ( ss.get() != '\n' )
					;
				break;

			case 'v':
				if ( ss.peek() == 'n' )
				{
					ss.get();

					Normal_t vec;
					for ( int i = 0; i < vec.length(); ++i )
					{
						ss >> vec[ i ];
					}

					normal_per_face.push_back( std::move( vec ) );
				}
				else if ( ss.peek() == 't' )
				{
					ss.get();

					Texture_t vec;
					for ( int i = 0; i < vec.length(); ++i )
					{
						ss >> vec[ i ];
					}


					texture_per_face.push_back( std::move( vec ) );
				}
				else // v
				{
					Pos_t vec;
					for ( int i = 0; i < vec.length(); ++i )
					{
						ss >> vec[ i ];
					}

					att_pos.push_back( std::move( vec ) );
					normals_per_vertex.push_back( {} );
					textures_per_vertex.push_back( {} );
				}
				break;

			case 'f':
				for ( size_t j = 0; j < index_per_face; ++j )
				{
					Idx_t first;
					Idx_t second;
					Idx_t third;

					ss >> first;
					ss.get();
					ss >> second;
					ss.get();
					ss >> third;

					--first;
					--second;
					--third;

					att_index.push_back( first );
					textures_per_vertex[ first ].push_back( texture_per_face[ second ] );
					normals_per_vertex[ first ].push_back( normal_per_face[ third ] );
				}
				break;
			}
		}

		for ( const auto& normals : normals_per_vertex )
		{
			Normal_t sum{ 0 };

			for ( const auto& normal : normals )
			{
				sum += normal;
			}

			if ( glm::length( sum ) < std::numeric_limits< Elem_t >::epsilon() * 10 )
			{
				std::cerr << "[ load_obj_interpret ] : face normals' average was too small.\n";
				sum = Normal_t{ 1.f };
			}

			att_normal.push_back( glm::normalize( sum / static_cast< Elem_t >( normals.size() ) ) );
		}

		for ( const auto& textures : textures_per_vertex )
		{
			Texture_t sum{ 0 };

			for ( const auto& texture : textures )
			{
				sum += texture;
			}

			att_texture.push_back( sum / static_cast< Elem_t >( textures.size() ) );
		}
	}
};

bool ComponentVertex::will_get_desc = true;

class ComponentRender
{
public:
	using Idx_t = ComponentVertex::Idx_t;

	// const GLenum mode, const Idx_t first, const Idx_t cnt
	struct DrawArrayDetail
	{
		const GLenum mode;
		const Idx_t first;
		const Idx_t cnt;
	};

	// const GLenum mode, const Idx_t cnt, const GLenum type, void* indices
	struct DrawElementsDetail
	{
		const GLenum mode;
		const Idx_t cnt;
		const GLenum type;
		void* indices;
	};

	CTexture texture;

public:
	static bool will_get_desc;

	const std::stringstream get_desc( const std::string& fore_indent = "") const
	{
		std::stringstream ss;

		if ( will_get_desc )
		{
			ss << fore_indent << typeid( *this ).name() << '\n';
			ss << get_draw_desc_da( fore_indent + indent ).str();
			ss << get_draw_desc_de( fore_indent + indent ).str();
		}

		return ss;
	}

	void append( const DrawArrayDetail& dad ) { DA_details.push_back( dad ); }
	void append( const DrawElementsDetail& ded ) { DE_details.push_back( ded ); }
	void clear() { DA_details.clear(); DE_details.clear(); }

	void draw( const ComponentVertex& vertex_comp_inst ) const
	{
		glUseProgram( vertex_comp_inst.shader_program_id );

		if ( texture ) texture->bind_this();

		for ( const auto& DA_detail : DA_details )
		{
			vertex_comp_inst.vao.draw_array( DA_detail.mode, DA_detail.first, DA_detail.cnt );
		}

		for ( const auto& DE_detail : DE_details )
		{
			vertex_comp_inst.vao.draw_elements( DE_detail.mode, DE_detail.cnt, DE_detail.type, DE_detail.indices );
		}

		if ( texture ) texture->unbind_this();
	}

	ComponentRender() : texture{} {}
	ComponentRender( const ComponentRender& ) = default;
	ComponentRender& operator=( const ComponentRender& ) = default;
	ComponentRender( ComponentRender&& ) noexcept = default;
	ComponentRender& operator=( ComponentRender&& ) noexcept = default;

private:
	std::vector< DrawArrayDetail > DA_details;
	std::vector< DrawElementsDetail > DE_details;

	const std::string mode_str( GLenum mode ) const
	{
		switch ( mode )
		{
		default:
			return "[[[[[ Wrong Mode Detected ]]]]]---------------------";

		case GL_TRIANGLES:
			return "GL_TRIANGLES";

		case GL_TRIANGLE_STRIP:
			return "GL_TRIANGLE_STRIP";

		case GL_TRIANGLE_FAN:
			return "GL_TRIANGLE_FAN";

		case GL_QUADS:
			return "GL_QUADS";

		case GL_QUAD_STRIP:
			return "GL_QUAD_STRIP";

		case GL_LINES:
			return "GL_LINES";

		case GL_LINE_STRIP:
			return "GL_LINE_STRIP";

		case GL_LINE_LOOP:
			return "GL_LINE_LOOP";
		}
	}

	const std::string type_str( GLenum type ) const
	{
		switch ( type )
		{
		default:
			return "[[[[[ Wrong Index Type Detected ]]]]]---------------------";

		case GL_INT:
			return "GL_INT";

		case GL_UNSIGNED_INT:
			return "GL_UNSIGNED_INT";

		case GL_SHORT:
			return "GL_SHORT";

		case GL_UNSIGNED_SHORT:
			return "GL_UNSIGNED_SHORT";

		case GL_BYTE:
			return "GL_BYTE";

		case GL_UNSIGNED_BYTE:
			return "GL_UNSIGNED_BYTE";

		case GL_INT64_ARB:
			return "GL_INT64";

		case GL_UNSIGNED_INT64_ARB:
			return "GL_UNSIGNED_INT64";
		}
	}

	const std::stringstream get_draw_desc_da( const std::string& fore_indent ) const
	{
		std::stringstream ss;
		int cnt = 0;

		for ( const auto& DA_detail : DA_details )
		{
			ss << fore_indent << ++cnt << Ord( cnt ) << "DrawArray Detail:\n";

			ss << fore_indent << indent << "mode: " << mode_str( DA_detail.mode ) << '\n';
			ss << fore_indent << indent << "first: " << DA_detail.first << '\n';
			ss << fore_indent << indent << "cnt: " << DA_detail.cnt << '\n';
		}

		return ss;
	}

	const std::stringstream get_draw_desc_de( const std::string& fore_indent ) const
	{
		std::stringstream ss;
		int cnt = 0;

		for ( const auto& DE_detail : DE_details )
		{
			ss << fore_indent << ++cnt << Ord( cnt ) << " DrawElements Detail:\n";

			ss << fore_indent << indent << "mode: " << mode_str( DE_detail.mode ) << '\n';
			ss << fore_indent << indent << "cnt: " << DE_detail.cnt << '\n';
			ss << fore_indent << indent << "type: " << type_str( DE_detail.type ) << '\n';
			ss << fore_indent << indent << "indices: " << DE_detail.indices << '\n';
		}

		return ss;
	}

};

bool ComponentRender::will_get_desc = true;

#endif