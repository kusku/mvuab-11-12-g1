#pragma once

#ifndef _XML_MANAGER_H
#define _XML_MANAGER_H

#include <string>

class CXMLTreeNode;
class CGuiElement;
class CGUIWindow;
class CTexture;

class CXMLManager
{
public:
	static void SaveFiles(const std::string &filename );

private:
	static void SaveWindow(CXMLTreeNode &node, CGUIWindow *window, const std::string &path);
	static void SavePointerMouse(CXMLTreeNode &node, CGuiElement *element, const std::string &path);

	static void SaveAnimatedImage(CXMLTreeNode &node, CGuiElement *element, const std::string &path);
	static void SaveButton(CXMLTreeNode &node, CGuiElement *element, const std::string &path);
	static void SaveCheckButton(CXMLTreeNode &node, CGuiElement *element, const std::string &path);
	static void SaveDialogBox(CXMLTreeNode &node, CGuiElement *element, const std::string &path);
	static void SaveEditableTextBox(CXMLTreeNode &node, CGuiElement *element, const std::string &path);
	static void SaveImage(CXMLTreeNode &node, CGuiElement *element, const std::string &path);

	static void WriteCommonProperties(CXMLTreeNode &node, CGuiElement *element);
	static std::string GetNameTexture(const std::string &file);
	static void CopyTexture(CTexture *texture, const std::string &path);
	static void CreateDirectories(const std::string &path);
};

#endif