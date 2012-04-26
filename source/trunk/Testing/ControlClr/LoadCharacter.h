#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;
using namespace DoubleAgent::Control;

namespace DoubleAgent {
/////////////////////////////////////////////////////////////////////////////

/// <summary>
/// Summary for LoadCharacter
///
/// WARNING: If you change the name of this class, you will need to change the
///          'Resource File Name' property for the managed resource compiler tool
///          associated with all .resx files this class depends on.  Otherwise,
///          the designers will not be able to interact properly with localized
///          resources associated with this form.
/// </summary>
public ref class LoadCharacter : public System::Windows::Forms::Form
{
public:
	LoadCharacter(DoubleAgent::AxControl::AxControl^ pDaControl)
	:	mDaControl (pDaControl)
	{
		InitializeComponent();
	}

protected:
	/// <summary>
	/// Clean up any resources being used.
	/// </summary>
	~LoadCharacter()
	{
		if (components)
		{
			delete components;
		}
	}
private: System::Windows::Forms::Button^  OkButton;
protected:
private: System::Windows::Forms::Button^  CancelButton;
private: System::Windows::Forms::TreeView^  CharacterTree;
private: System::Windows::Forms::Label^  CharacterIDLabel;
private: System::Windows::Forms::TextBox^  CharacterID;

private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(LoadCharacter::typeid));
			this->OkButton = (gcnew System::Windows::Forms::Button());
			this->CancelButton = (gcnew System::Windows::Forms::Button());
			this->CharacterTree = (gcnew System::Windows::Forms::TreeView());
			this->CharacterIDLabel = (gcnew System::Windows::Forms::Label());
			this->CharacterID = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			//
			// OkButton
			//
			this->OkButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->OkButton->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->OkButton->Enabled = false;
			this->OkButton->Location = System::Drawing::Point(363, 12);
			this->OkButton->Name = L"OkButton";
			this->OkButton->Size = System::Drawing::Size(108, 32);
			this->OkButton->TabIndex = 1;
			this->OkButton->Text = L"OK";
			this->OkButton->UseVisualStyleBackColor = true;
			//
			// CancelButton
			//
			this->CancelButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->CancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->CancelButton->Location = System::Drawing::Point(363, 50);
			this->CancelButton->Name = L"CancelButton";
			this->CancelButton->Size = System::Drawing::Size(108, 32);
			this->CancelButton->TabIndex = 2;
			this->CancelButton->Text = L"Cancel";
			this->CancelButton->UseVisualStyleBackColor = true;
			//
			// CharacterTree
			//
			this->CharacterTree->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->CharacterTree->Location = System::Drawing::Point(12, 12);
			this->CharacterTree->Name = L"CharacterTree";
			this->CharacterTree->Size = System::Drawing::Size(341, 491);
			this->CharacterTree->TabIndex = 3;
			this->CharacterTree->NodeMouseDoubleClick += gcnew System::Windows::Forms::TreeNodeMouseClickEventHandler(this, &LoadCharacter::CharacterTree_NodeMouseDoubleClick);
			this->CharacterTree->AfterSelect += gcnew System::Windows::Forms::TreeViewEventHandler(this, &LoadCharacter::CharacterTree_AfterSelect);
			//
			// CharacterIDLabel
			//
			this->CharacterIDLabel->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->CharacterIDLabel->AutoSize = true;
			this->CharacterIDLabel->Location = System::Drawing::Point(12, 512);
			this->CharacterIDLabel->Name = L"CharacterIDLabel";
			this->CharacterIDLabel->Size = System::Drawing::Size(87, 17);
			this->CharacterIDLabel->TabIndex = 4;
			this->CharacterIDLabel->Text = L"Character ID";
			//
			// CharacterID
			//
			this->CharacterID->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->CharacterID->Location = System::Drawing::Point(112, 509);
			this->CharacterID->Name = L"CharacterID";
			this->CharacterID->Size = System::Drawing::Size(241, 22);
			this->CharacterID->TabIndex = 0;
			this->CharacterID->Validating += gcnew System::ComponentModel::CancelEventHandler(this, &LoadCharacter::CharacterID_Validating);
			//
			// LoadCharacter
			//
			this->AcceptButton = this->OkButton;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(483, 543);
			this->Controls->Add(this->CharacterID);
			this->Controls->Add(this->CharacterIDLabel);
			this->Controls->Add(this->CharacterTree);
			this->Controls->Add(this->CancelButton);
			this->Controls->Add(this->OkButton);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"LoadCharacter";
			this->ShowInTaskbar = false;
			this->Text = L"Load Character";
			this->Load += gcnew System::EventHandler(this, &LoadCharacter::LoadCharacter_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &LoadCharacter::LoadCharacter_FormClosed);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public:
		String^								mFilePath;
		String^								mCharacterID;
	private:
		DoubleAgent::AxControl::AxControl^	mDaControl;

	private: System::Void LoadCharacter_Load(System::Object^ sender, System::EventArgs^ e)
	{
		try
		{
			DoubleAgent::Control::CharacterFiles^		lFiles = mDaControl->CharacterFiles;
			String^										lFilePath;
			String^										lPathName;
			String^										lFileName;
			TreeNode^									lRootNode;
			Generic::Dictionary <String^, TreeNode^>^	lRootNodes = gcnew Generic::Dictionary <String^, TreeNode^>;

			lFiles->DoubleAgentFiles = true;
			lFiles->MsAgentFiles = true;
			lFiles->MsOfficeFiles = true;
			lFiles->CompliantCharacters = true;
			lFiles->NonCompliantCharacters = true;
			lFiles->SpeakingCharacters = true;
			lFiles->NonSpeakingCharacters = true;
			lFiles->VerifyVersion = true;

			CharacterTree->BeginUpdate();

			lRootNode = CharacterTree->Nodes->Add ("<default>");

			for each (lFilePath in lFiles->FilePaths)
			{
				lPathName = Path::GetDirectoryName (lFilePath);
				lFileName = Path::GetFileName (lFilePath);

				if	(lRootNodes->ContainsKey (lPathName))
				{
					lRootNode = lRootNodes [lPathName];
				}
				else
				{
					lRootNode = CharacterTree->Nodes->Add (lPathName);
					lRootNodes [lPathName] = lRootNode;
				}
				lRootNode->Nodes->Add (lFileName);
				lRootNode->Expand ();
			}

			CharacterTree->EndUpdate();
		}
		catch AnyExceptionDebug
	}

	private: System::Void CharacterTree_NodeMouseDoubleClick(System::Object^  sender, System::Windows::Forms::TreeNodeMouseClickEventArgs^  e)
	{
		if	(
				(e->Node)
//			&&	(e->Node->Level > 0)
			&&	(CharacterTree->SelectedNode)
			&&	(
					(CharacterTree->SelectedNode->Level > 0)
				||	(CharacterTree->SelectedNode->PrevNode == nullptr)
				)
			)
		{
			DialogResult = System::Windows::Forms::DialogResult::OK;
			Close ();
		}
	}

	private: System::Void CharacterTree_AfterSelect(System::Object^  sender, System::Windows::Forms::TreeViewEventArgs^  e)
	{
		if	(
				(e->Node)
			&&	(e->Node->Level > 0)
			)
		{
			CharacterID->Text = Path::GetFileNameWithoutExtension (e->Node->Text);
			OkButton->Enabled = true;
		}
		else
		if	(
				(e->Node)
			&&	(e->Node->PrevNode == nullptr)
			)
		{
			CharacterID->Text = e->Node->Text;
			OkButton->Enabled = true;
		}
		else
		{
			OkButton->Enabled = false;
		}
	}

	private: System::Void CharacterID_Validating(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e)
	{
		if	(String::IsNullOrEmpty (CharacterID->Text))
		{
			if	(
					(CharacterTree->SelectedNode)
				&&	(CharacterTree->SelectedNode->Level > 0)
				)
			{
				CharacterID->Text = Path::GetFileNameWithoutExtension (CharacterTree->SelectedNode->Text);
				OkButton->Enabled = true;
			}
			else
			if	(
					(CharacterTree->SelectedNode)
				&&	(CharacterTree->SelectedNode->PrevNode == nullptr)
				)
			{
				CharacterID->Text = CharacterTree->SelectedNode->Text;
				OkButton->Enabled = true;
			}
			else
			{
				OkButton->Enabled = false;
			}
		}
	}
private: System::Void LoadCharacter_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
	{
		if	(DialogResult == System::Windows::Forms::DialogResult::OK)
		{
			if	(
					(CharacterTree->SelectedNode)
				&&	(CharacterTree->SelectedNode->Level > 0)
				)
			{
				mFilePath = Path::Combine (CharacterTree->SelectedNode->Parent->Text, CharacterTree->SelectedNode->Text);
			}
			else
			if	(
					(CharacterTree->SelectedNode)
				&&	(CharacterTree->SelectedNode->PrevNode == nullptr)
				)
			{
				mFilePath = String::Empty;
			}
			mCharacterID = CharacterID->Text;
		}
	}
};
/////////////////////////////////////////////////////////////////////////////
} // namespace DoubleAgent