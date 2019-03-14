/********************************************************************
    创建时间：    2016/10/17  13:35:58 
    文件名：      MyGumbo.h
    作者：        谢小军
    
    功能：        1.HTML解析工具
                2.
                
    说明：        1. 
                2. 
*********************************************************************/
#include "src/gumbo.h"
#include <string.h>
#include <string>
#include <assert.h>

//tools 工具类
namespace CMyGumboTools {
// 	inline std::string handle_unknown_tag(GumboStringPiece *text)
// 	{
// 		std::string tagname = "";
// 		if (text->data == NULL) {
// 			return tagname;
// 		}
// 		// work with copy GumboStringPiece to prevent asserts 
// 		// if try to read same unknown tag name more than once
// 		GumboStringPiece gsp = *text;
// 		gumbo_tag_from_original_text(&gsp);
// 		tagname = std::string(gsp.data, gsp.length);
// 		return tagname;
// 
// 	}

	//获取标签名字
	inline std::string get_tag_name(GumboNode *node)
	{
		if (!node)
			return "";

		std::string tagname;
		// work around lack of proper name for document node
		if (node->type == GUMBO_NODE_DOCUMENT) {
			tagname = "document";
		}
		else {
			tagname = gumbo_normalized_tagname(node->v.element.tag);
		}
		if (tagname.empty()) {
			tagname = "";
			GumboStringPiece *text = &node->v.element.original_tag;
			if (text)
			{
				if (text->data == NULL) {
					return tagname;
				}

				GumboStringPiece gsp = *text;
				gumbo_tag_from_original_text(&gsp);
				tagname = std::string(gsp.data, gsp.length);
			}
		}
		return tagname;
	}

	inline bool Equal(std::string key1, const std::string & key)
	{
		return (key1 == key);
	}
}
namespace CMyGumbo 
{
	//初始化Html头
	inline GumboOutput * CreatGumbo(const std::string & html )
	{
		return gumbo_parse_with_options(&kGumboDefaultOptions, html.c_str(), html.length()+1);
	}

	//释放
	inline void Release(GumboOutput *& output)
	{
		if (output)
		{
			gumbo_destroy_output(&kGumboDefaultOptions, output);
			output = nullptr;
		}
	}

	//获取头节点
	inline GumboNode* GetHead(GumboNode* node) {
		GumboNode* head = NULL;
		if (node)
		{
			assert(node->type == GUMBO_NODE_ELEMENT);
			assert(node->v.element.children.length >= 2);

			const GumboVector* root_children = &node->v.element.children;

			for (int i = 0; i < root_children->length; ++i) {
				GumboNode* child = (GumboNode*)root_children->data[i];
				if (child->type == GUMBO_NODE_ELEMENT &&
					child->v.element.tag == GUMBO_TAG_HEAD) {
					head = child;
					break;
				}
			}
			assert(head != NULL);
		}

		return head;
	}

	//获取头节点
	inline GumboNode* GetBody(GumboNode* node) {
		GumboNode* body = NULL;
		if (node)
		{
			assert(node->type == GUMBO_NODE_ELEMENT);
			assert(node->v.element.children.length >= 2);

			const GumboVector* root_children = &node->v.element.children;

			for (int i = 0; i < root_children->length; ++i) {
				GumboNode* child = (GumboNode*)root_children->data[i];
				if (child->type == GUMBO_NODE_ELEMENT &&
					child->v.element.tag == GUMBO_TAG_BODY) {
					body = child;
					break;
				}
			}
			assert(body != NULL);
		}

		return body;
	}

	/*
		查找节点
		node:父节点
		key:要查找的节点名字
		n:查找第几个节点
	*/
	inline GumboNode* Find(GumboNode* node, std::string key, int n=1)
	{
		GumboNode* element = nullptr;
		if (node)
		{
			GumboVector* head_children = &node->v.element.children;
			if (!head_children)
				return nullptr;

			int index = 0;
			for (int i = 0; i < head_children->length; ++i) {
				GumboNode* child = (GumboNode*)head_children->data[i];
				if (child && child->type == GUMBO_NODE_ELEMENT && CMyGumboTools::get_tag_name(child) == key)
				{
					index++;
					if (index >= n)//找到了
					{
						element = child;
						break;
					}
				}
			}
		}

		return element;
	}

	inline GumboNode* Find(GumboNode* node, int n = 1)
	{
		GumboNode* element = nullptr;
		if (node)
		{
			GumboVector* head_children = &node->v.element.children;
			if (!head_children)
				return nullptr;

			int index = 0;
			for (int i = 0; i < head_children->length; ++i) {
				GumboNode* child = (GumboNode*)head_children->data[i];
				if (child)
				{
					index++;
					if (index >= n)//找到了
					{
						element = child;
						break;
					}
				}
			}
		}

		return element;
	}

	//获取值
	inline std::string GetText(GumboNode* node)
	{
		GumboNode* title_text = node;
		if (!title_text)
			return "";

		assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
		if (title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE)
		{
			return std::string(title_text->v.text.text);
		}
		else
		{
			return "";
		}
		
	}

	//获取InnerHTML
	inline std::string GetInnerHTML(GumboNode* node)
	{
		GumboNode* title_text = node;
		if (!title_text)
			return "";

		assert(title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE);
		if (title_text->type == GUMBO_NODE_TEXT || title_text->type == GUMBO_NODE_WHITESPACE)
		{
			return std::string(title_text->v.text.original_text.data);
		}
		else
		{
			return "";
		}
	}

	//获取属性
	inline GumboAttribute* GetAttribute(GumboNode* node,const std::string & key)
	{
		if (node->type != GUMBO_NODE_ELEMENT) {
			return nullptr;
		}
		return gumbo_get_attribute(&node->v.element.attributes, key.c_str());
	}

	inline std::string GetText(GumboAttribute* node)
	{
		if (node)
		{
			return node->value;
		}

		return "";
	}

}