#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace DoubleAgent {
namespace TlbToAsm {

	/// <summary>
	/// Summary for AssemblyDlg
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class AssemblyDlg : public System::Windows::Forms::Form
	{
	protected:
		const int AssemblyImage;
		const int FolderClosedImage;
		const int ClassImage;
		const int InterfaceImage;
		const int EnumImage;
		const int MethodImage;
		const int FunctionImage;
		const int EventImage;
		const int OtherImage;

	public:
		AssemblyDlg(void)
		:	AssemblyImage (0),
			FolderClosedImage (1),
			ClassImage (2),
			InterfaceImage (3),
			EnumImage (4),
			MethodImage (5),
			FunctionImage (6),
			EventImage (7),
			OtherImage (8)
		{
			InitializeComponent();
			Application::EnableVisualStyles ();
		}

		void ShowAssembly (System::Reflection::Assembly^ pAssembly)
		{
			if	(pAssembly == nullptr)
			{
				this->Text = "<nullptr>";
			}
			else
			{
				this->Text = pAssembly->FullName;
				ShowAssemblyTree (pAssembly);
			}
		}

	protected:
		enum class TreeImages
		{
			AssemblyImage = 0,
			ClassImage = 2,
			InterfaceImage = 3,
			EnumImage = 4,
			MethodImage = 5,
			FunctionImage = 6,
			EventImage = 7,
			OtherImage = 8
		};

		void ShowAssemblyTree (System::Reflection::Assembly^ pAssembly);
		void ShowReferences (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::Assembly^ pAssembly);
		void ShowTypes (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::Assembly^ pAssembly);

		System::Windows::Forms::TreeNode^ ShowType (System::Windows::Forms::TreeNode^ pParentNode, System::Type^ pType, System::Boolean pExpand, System::String^ pNodeName);
		System::Windows::Forms::TreeNode^ ShowType (System::Windows::Forms::TreeNode^ pParentNode, System::Type^ pType, System::Boolean pExpand) {return ShowType (pParentNode, pType, pExpand, nullptr);}
		System::Windows::Forms::TreeNode^ ShowType (System::Windows::Forms::TreeNode^ pParentNode, System::Type^ pType) {return ShowType (pParentNode, pType, true, nullptr);}
		System::Windows::Forms::TreeNode^ ShowField (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::FieldInfo^ pField);
		System::Windows::Forms::TreeNode^ ShowProperty (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::PropertyInfo^ pProperty);
		System::Windows::Forms::TreeNode^ ShowMethod (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::MethodInfo^ pMethod);
		System::Windows::Forms::TreeNode^ ShowEvent (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::EventInfo^ pEvent);
		System::Windows::Forms::TreeNode^ ShowParameter (System::Windows::Forms::TreeNode^ pParentNode, System::Reflection::ParameterInfo^ pParameter);
		void ShowAttributes (System::Windows::Forms::TreeNode^ pParentNode, System::Collections::Generic::IList <System::Reflection::CustomAttributeData^>^ pAttributes);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~AssemblyDlg()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  OkButton;
	private: System::Windows::Forms::Button^  CancelButton;
	private: System::Windows::Forms::TreeView^  AssemblyTree;
	private: System::Windows::Forms::ImageList^  AssemblyImages;
	private: System::ComponentModel::IContainer^  components;
	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(AssemblyDlg::typeid));
			this->OkButton = (gcnew System::Windows::Forms::Button());
			this->CancelButton = (gcnew System::Windows::Forms::Button());
			this->AssemblyTree = (gcnew System::Windows::Forms::TreeView());
			this->AssemblyImages = (gcnew System::Windows::Forms::ImageList(this->components));
			this->SuspendLayout();
			// 
			// OkButton
			// 
			this->OkButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->OkButton->Location = System::Drawing::Point(868, 14);
			this->OkButton->Margin = System::Windows::Forms::Padding(4);
			this->OkButton->Name = L"OkButton";
			this->OkButton->Size = System::Drawing::Size(128, 38);
			this->OkButton->TabIndex = 0;
			this->OkButton->Text = L"OK";
			this->OkButton->UseVisualStyleBackColor = true;
			// 
			// CancelButton
			// 
			this->CancelButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->CancelButton->Location = System::Drawing::Point(868, 58);
			this->CancelButton->Margin = System::Windows::Forms::Padding(4);
			this->CancelButton->Name = L"CancelButton";
			this->CancelButton->Size = System::Drawing::Size(128, 38);
			this->CancelButton->TabIndex = 1;
			this->CancelButton->Text = L"Cancel";
			this->CancelButton->UseVisualStyleBackColor = true;
			// 
			// AssemblyTree
			// 
			this->AssemblyTree->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->AssemblyTree->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->AssemblyTree->CausesValidation = false;
			this->AssemblyTree->HideSelection = false;
			this->AssemblyTree->ImageIndex = 8;
			this->AssemblyTree->ImageList = this->AssemblyImages;
			this->AssemblyTree->ItemHeight = 20;
			this->AssemblyTree->Location = System::Drawing::Point(13, 14);
			this->AssemblyTree->Margin = System::Windows::Forms::Padding(4);
			this->AssemblyTree->Name = L"AssemblyTree";
			this->AssemblyTree->SelectedImageIndex = 0;
			this->AssemblyTree->Size = System::Drawing::Size(848, 662);
			this->AssemblyTree->TabIndex = 2;
			// 
			// AssemblyImages
			// 
			this->AssemblyImages->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"AssemblyImages.ImageStream")));
			this->AssemblyImages->TransparentColor = System::Drawing::Color::Transparent;
			this->AssemblyImages->Images->SetKeyName(0, L"Assembly.ico");
			this->AssemblyImages->Images->SetKeyName(1, L"FolderClosed.ico");
			this->AssemblyImages->Images->SetKeyName(2, L"Class.ico");
			this->AssemblyImages->Images->SetKeyName(3, L"COM.ico");
			this->AssemblyImages->Images->SetKeyName(4, L"Enum.ico");
			this->AssemblyImages->Images->SetKeyName(5, L"Method.ico");
			this->AssemblyImages->Images->SetKeyName(6, L"Function.ico");
			this->AssemblyImages->Images->SetKeyName(7, L"Event.ico");
			this->AssemblyImages->Images->SetKeyName(8, L"Other.ico");
			// 
			// AssemblyDlg
			// 
			this->AcceptButton = this->OkButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(9, 18);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoValidate = System::Windows::Forms::AutoValidate::Disable;
			this->CancelButton = this->CancelButton;
			this->CausesValidation = false;
			this->ClientSize = System::Drawing::Size(1010, 689);
			this->Controls->Add(this->AssemblyTree);
			this->Controls->Add(this->CancelButton);
			this->Controls->Add(this->OkButton);
			this->Font = (gcnew System::Drawing::Font(L"Verdana", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Margin = System::Windows::Forms::Padding(4);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"AssemblyDlg";
			this->Text = L"Assembly";
			this->ResumeLayout(false);

		}
#pragma endregion
};
} // namespace TlbToAsm
} // namespace DoubleAgent
