/*

Copyright (c) 2012 Julik Tarkhanov

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
static const char* const CLASS = "JT_UVTransform";
static const char* const HELP = "This node will apply simpe 2D transforms to the UV coordinates. If you have problems with the plugin, contact me@julik.nl";

#include "DDImage/ModifyGeo.h"
#include "DDImage/Scene.h"
#include "DDImage/Knob.h"
#include "DDImage/Knobs.h"
#include <sstream>

#define VERSION "1.0.0"

using namespace DD::Image;

class JT_UVTransform : public ModifyGeo
{
private:
	
	// Transform applied to UVs
	Matrix4 xform;
	
	// UV attribute name
	const char* uv_attrib_name;
	
	// Group type ptr detected in keep_uvs()
	DD::Image::GroupType t_group_type;
	
	bool wrap_, flip_, flop_;
	
public:

	static const Description description;
	
	const char* Class() const
	{
		return CLASS;
	}
	
	const char* node_help() const
	{
		return HELP;
	}

	JT_UVTransform(Node* node) : ModifyGeo(node)
	{
		uv_attrib_name = "uv";
		wrap_ = false;
		flip_ = false;
		flop_ = false;
	}
	
	Matrix4* matrix() { return &xform; }
	
	void knobs(Knob_Callback f)
	{
		ModifyGeo::knobs(f);
		
		// Transform configuration
		Knob* flip_knob = Bool_knob( f, &flip_, "flip");
		flip_knob->label("mirror U");
		flip_knob->tooltip("Will mirror the texture coordinates horizontally");
//		flip_knob->set_flag(KNOB_ON_SEPARATE_LINE);

		Knob* flop_knob = Bool_knob( f, &flop_, "flop");
		flop_knob->label("mirror V");
		flop_knob->tooltip("Will mirror the texture coordinates vertically");
		
		Knob* wrap_knob = Bool_knob( f, &wrap_, "wrap");
		wrap_knob->label("tile UV");
		wrap_knob->tooltip("Will tile the generated texture coordinates");
		
		// All the transformations
		Knob* _xformKnob = Axis_knob(f, &xform, "xform");
		
		Divider(f, 0);
		std::ostringstream ver;
		ver << "JT_UVTransform v." << VERSION;
		Text_knob(f, ver.str().c_str());
	}
	
	void append_transform_to(Hash& hash)
	{
		xform.append(hash);
		knob("xform")->append(hash, NULL);
		hash.append(flip_);
		hash.append(flop_);
		hash.append(wrap_);
	}
	
	void get_geometry_hash()
	{
		// Get all hashes up-to-date
		ModifyGeo::get_geometry_hash();
		append_transform_to(geo_hash[Group_Points]);
		append_transform_to(geo_hash[Group_Vertices]);
	}
	
	void append(Hash& hash) {
		hash.append(VERSION);
		hash.append(uv_attrib_name);
		append_transform_to(hash);
		ModifyGeo::append(hash); // the super called he wants his pointers back
	}
	
	// This is needed to preserve UVs which are already there
	void keep_uvs(int index, GeoInfo& info, GeometryList& out)
	{
		
		// get the original uv attribute used to restore untouched uv coordinate
		const AttribContext* context = info.get_attribcontext(uv_attrib_name);
		AttributePtr uv_original = context ? context->attribute : AttributePtr();
		
		if(!uv_original){
			Op::error( "Missing \"%s\" channel from geometry", uv_attrib_name );
			return;
		}

		t_group_type = context->group; // texture coordinate group type

		// we have two possibilities:
		// the uv coordinate are stored in Group_Points or in Group_Vertices way
		// sanity check
		assert(t_group_type == Group_Points || t_group_type == Group_Vertices);
		
		// create a buffer to write on it
		Attribute* uv = out.writable_attribute(index, t_group_type, uv_attrib_name, VECTOR4_ATTRIB);
		assert(uv);

		// copy all original texture coordinate if available
		if (uv_original){
			// sanity check
			assert(uv->size() == uv_original->size());

			for (unsigned i = 0; i < uv->size(); i++) {
				uv->vector4(i) = uv_original->vector4(i);
			}
		}
	}
	
	float wrap(float n)
	{
		float mod = fmod(n, 1.0f);
		if(mod < 0.0f) {
			mod = 1.0f + mod;
		}
		return mod;
	}
	
	void transform_uv(Vector4& uvw)
	{
		double w = uvw.w;
		
		uvw.x = uvw.x / uvw.w;
		uvw.y = uvw.y / uvw.w;
		uvw.z = 0;
		uvw.w = 0;

		
		// Apply rotation, scale, skew
		Vector4 xformed = xform * uvw;
		
		uvw.set(xformed.x, xformed.y, xformed.z, xformed.w);
		
		if(flip_) {
			uvw.x = 1.0f - uvw.x;
		}
		
		if(flop_) {
			uvw.y = 1.0f - uvw.y;
		}

		if(wrap_) {
			uvw.x = wrap(uvw.x);
			uvw.y = wrap(uvw.y);
		}
		
		// Remult by W
		uvw.x *= w;
		uvw.y *= w;
		uvw.w = w;
	}
	
	// Apply distortion to each element in the passed vertex or point attribute
	void transform_each_element_in_attribute(Attribute* attr, unsigned const int num_of_elements)
	{
		if(!attr) return;
		for (unsigned point_idx = 0; point_idx < num_of_elements; point_idx++) {
			transform_uv(attr->vector4(point_idx));
		}
	}
	
	void modify_geometry(int obj, Scene& scene, GeometryList& out)
	{
		// Call the engine on all the caches:
		for (unsigned i = 0; i < out.objects(); i++) {
			GeoInfo& info = out[i];
			
			// Copy over old UV attributes
			keep_uvs(i, info, out);
			
			// Reusable pointer for the attribute we are going to be writing to
			Attribute* uv;
			
			// Copy over pt attributes
			uv = out.writable_attribute(i, Group_Points, uv_attrib_name, VECTOR4_ATTRIB);
			transform_each_element_in_attribute(uv, info.points());
			
			// If the previously detected group type is vertex attribute we need to distort it as well
			// since vertex attribs take precedence and say a Sphere in Nuke has vertex attribs
			// as opposed to point attribs :-( so justified double work here
			if(t_group_type == Group_Vertices) {
				uv = out.writable_attribute(i, Group_Vertices, uv_attrib_name, VECTOR4_ATTRIB);
				transform_each_element_in_attribute(uv, info.vertices()); // Copy over vertex attributes
			}
		}
	}
};

static Op* build(Node* node)
{
	return new JT_UVTransform(node);
}
const Op::Description JT_UVTransform::description(CLASS, build);

