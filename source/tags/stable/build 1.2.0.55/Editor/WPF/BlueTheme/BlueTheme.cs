using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Resources;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Media.Effects;

namespace DoubleAgent.Theme
{
	public class BlueTheme : ResourceDictionary
	{
		/////////////////////////////////////////////////////////////////////////////
		#region Initialization

		public BlueTheme ()
		{
			LoadTheme ();
#if DEBUG_NOT
			DumpResourceKeys ();
#endif
		}

		private void LoadTheme ()
		{
			try
			{
				System.Uri lResourceLocater = new System.Uri ("/BlueTheme;component/BlueTheme/BlueTheme.xaml", System.UriKind.Relative);
				ResourceDictionary lDictionary = System.Windows.Application.LoadComponent (lResourceLocater)as ResourceDictionary;

				if (lDictionary != null)
				{
					ResourceDictionary[] lMergedDictionaries = new ResourceDictionary[lDictionary.MergedDictionaries.Count]; 
					lDictionary.MergedDictionaries.CopyTo (lMergedDictionaries, 0);
					lDictionary.MergedDictionaries.Clear ();

					foreach (ResourceDictionary lMergedDictionary in lMergedDictionaries)
					{
						MergedDictionaries.Add (lMergedDictionary);
					}
				}
			}
			catch (Exception pException)
			{
				Debug.Print (pException.Message);
			}
		}

#if false
		private void LoadTheme ()
		{
			try
			{
				Assembly lAssembly = GetType ().Assembly;

				foreach (String lResourceName in lAssembly.GetManifestResourceNames ())
				{
					try
					{
						if ((lAssembly.GetManifestResourceInfo (lResourceName).ResourceLocation & System.Reflection.ResourceLocation.ContainedInAnotherAssembly) == 0)
						{
#if DEBUG
							Debug.Print ("Resource [{0}] [{1}]", lResourceName, lAssembly.GetManifestResourceInfo (lResourceName).ResourceLocation);
#endif
							LoadResourceBaml (lAssembly.GetManifestResourceStream (lResourceName));
						}
					}
					catch (Exception pException)
					{
						Debug.Print (pException.Message);
					}

				}
			}
			catch (Exception pException)
			{
				Debug.Print (pException.Message);
			}
		}

		private void LoadResourceBaml (Stream pResourceStream)
		{
			if (pResourceStream != null)
			{
				try
				{
					using (System.Resources.ResourceReader lResourceReader = new System.Resources.ResourceReader (pResourceStream))
					{
						foreach (System.Collections.DictionaryEntry lResourceEntry in lResourceReader)
						{
							if (lResourceEntry.Key.ToString ().ToLower ().Contains ("shared"))
							{
								LoadResourceBaml (lResourceEntry);
							}
						}
						foreach (System.Collections.DictionaryEntry lResourceEntry in lResourceReader)
						{
							if (!lResourceEntry.Key.ToString ().ToLower ().Contains ("shared"))
							{
								LoadResourceBaml (lResourceEntry);
							}
						}
					}
				}
				catch (Exception pException)
				{
					Debug.Print (pException.Message);
				}
			}
		}

		private void LoadResourceBaml (DictionaryEntry pResourceEntry)
		{
			if (pResourceEntry.Value is Stream)
			{
				try
				{
					System.Xaml.XamlReaderSettings lReaderSettings;
					System.Windows.Baml2006.Baml2006Reader lBamlReader;
					Object lRootObject;
#if DEBUG_NOT
					Debug.Print ("  Read [{0}]", lEntry.Key);
#endif
					lReaderSettings = new System.Xaml.XamlReaderSettings ();
					lReaderSettings.LocalAssembly = GetType ().Assembly;
					lBamlReader = new System.Windows.Baml2006.Baml2006Reader (pResourceEntry.Value as Stream, lReaderSettings);
					lRootObject = System.Windows.Markup.XamlReader.Load (lBamlReader);

					if (lRootObject is ResourceDictionary)
					{
#if DEBUG
						Debug.Print ("  ResourceDictionary [{0}]", pResourceEntry.Key);
#endif
						MergedDictionaries.Add (lRootObject as ResourceDictionary);
					}
				}
				catch (Exception pException)
				{
					Debug.Print (pException.Message);
				}
			}
		}
#endif
#if DEBUG
		private void DumpResourceKeys ()
		{
			try
			{
				Debug.Print ("BlueTheme...");

				foreach (Object lKey in Keys)
				{
					Debug.Print ("  Key [{0}]", lKey);
				}
				foreach (ResourceDictionary lDict in MergedDictionaries)
				{
					foreach (Object lKey in lDict.Keys)
					{
						Debug.Print ("  Key [{0}]", lKey);
					}
				}

				Debug.Print ("BlueTheme...End");
			}
			catch (Exception pException)
			{
				Debug.Print (pException.Message);
			}
		}
#endif
		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Theme Constants

		public static Duration StateAnimationDuration
		{
			get
			{
				return new Duration (TimeSpan.FromSeconds (0.2));
			}
		}
		public static Duration FastAnimationDuration
		{
			get
			{
				return new Duration (TimeSpan.FromSeconds (0.15));
			}
		}
		public static Duration ToggleButtonTransitionDuration
		{
			get
			{
				return new Duration (TimeSpan.FromSeconds (0.2));
			}
		}
		public static Double ItemDisabledOpacity
		{
			get
			{
				return 0.3;
			}
		}

		#endregion
		/////////////////////////////////////////////////////////////////////////////
		#region Resource Keys

		public static ResourceKey SelectionPaddingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Sel_0001");
			}
		}
		public static ResourceKey SelectionMarginKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Sel_0002");
			}
		}
		public static ResourceKey SelectionStrokeKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Double), "Blue_Sel_0003");
			}
		}
		public static ResourceKey SelectionRoundingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Double), "Blue_Sel_0004");
			}
		}

		public static ResourceKey SelectionOuterColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0005");
			}
		}
		public static ResourceKey SelectionTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0006");
			}
		}
		public static ResourceKey SelectionBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0007");
			}
		}
		public static ResourceKey SelectionHighlitedOuterColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0008");
			}
		}
		public static ResourceKey SelectionHighlitedTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0009");
			}
		}
		public static ResourceKey SelectionHighlitedBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0010");
			}
		}
		public static ResourceKey SelectionUnfocusedOuterColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0011");
			}
		}
		public static ResourceKey SelectionUnfocusedTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0012");
			}
		}
		public static ResourceKey SelectionUnfocusedBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Sel_0013");
			}
		}

		public static ResourceKey SelectionOuterBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0014");
			}
		}
		public static ResourceKey SelectionFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0015");
			}
		}
		public static ResourceKey SelectionHighlitedOuterBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0016");
			}
		}
		public static ResourceKey SelectionHighlitedFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0017");
			}
		}
		public static ResourceKey SelectionUnfocusedOuterBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0018");
			}
		}
		public static ResourceKey SelectionUnfocusedFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0019");
			}
		}

		public static ResourceKey SelectionInnerBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Sel_0020");
			}
		}
		public static ResourceKey SelectionInnerOpacityKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Double), "Blue_Sel_0021");
			}
		}

		//=============================================================================

		public static ResourceKey BorderNormalColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Border_0001");
			}
		}
		public static ResourceKey BorderFocusedColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Border_0002");
			}
		}
		public static ResourceKey BorderNormalBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Border_0003");
			}
		}
		public static ResourceKey BorderFocusedBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Border_0004");
			}
		}
		public static ResourceKey BorderNormalThicknessKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Border_0005");
			}
		}
		public static ResourceKey BorderNormalPaddingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Border_0006");
			}
		}
		public static ResourceKey BorderFocusedThicknessKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Border_0007");
			}
		}
		public static ResourceKey BorderFocusedPaddingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Border_0008");
			}
		}
		public static ResourceKey BorderMarginKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Border_0009");
			}
		}
		public static ResourceKey BorderRoundingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (CornerRadius), "Blue_Border_0010");
			}
		}

		public static ResourceKey BorderNormalStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Border_0011");
			}
		}
		public static ResourceKey BorderFocusedStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Border_0012");
			}
		}

		public static ResourceKey BorderFocusedShowAnimationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Storyboard), "Blue_Border_0013");
			}
		}
		public static ResourceKey BorderFocusedHideAnimationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Storyboard), "Blue_Border_0014");
			}
		}
		public static ResourceKey BorderHitShowAnimationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Storyboard), "Blue_Border_0015");
			}
		}
		public static ResourceKey BorderHitHideAnimationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Storyboard), "Blue_Border_0016");
			}
		}

		//=============================================================================

		public static ResourceKey ButtonGlowShowAnimationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Storyboard), "Blue_Button_0001");
			}
		}
		public static ResourceKey ButtonGlowHideAnimationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Storyboard), "Blue_Button_0002");
			}
		}

		//=============================================================================

		public static ResourceKey PushButtonTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Push_0001");
			}
		}
		public static ResourceKey PushButtonBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Push_0002");
			}
		}
		public static ResourceKey PushButtonHighlitedTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Push_0003");
			}
		}
		public static ResourceKey PushButtonHighlitedBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Push_0004");
			}
		}

		public static ResourceKey PushButtonBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Push_0005");
			}
		}
		public static ResourceKey PushButtonFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Push_0006");
			}
		}
		public static ResourceKey PushButtonHighlitedFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Push_0007");
			}
		}

		public static ResourceKey PushButtonDefaultPaddingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Push_0008");
			}
		}
		public static ResourceKey PushButtonBorderPaddingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Push_0009");
			}
		}
		public static ResourceKey PushButtonBorderThicknessKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Push_0010");
			}
		}
		public static ResourceKey PushButtonBorderPaddingPressedKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Push_0011");
			}
		}
		public static ResourceKey PushButtonBorderThicknessPressedKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Push_0012");
			}
		}
		public static ResourceKey PushButtonRoundingKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (CornerRadius), "Blue_Push_0013");
			}
		}
		public static ResourceKey PushButtonTransitionDurationKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Duration), "Blue_Push_0014");
			}
		}
		public static ResourceKey PushButtonGlowMarginKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Push_0015");
			}
		}
		public static ResourceKey PushButtonGlowEffectKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Effect), "Blue_Push_0016");
			}
		}

		public static ResourceKey PushButtonFocusVisualKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Push_0017");
			}
		}

		//=============================================================================

		public static ResourceKey ToggleButtonCheckmarkKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Toggle_0001");
			}
		}
		public static ResourceKey ToggleButtonUncheckmarkKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Toggle_0002");
			}
		}
		public static ResourceKey ToggleButtonCheckMarginKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Toggle_0003");
			}
		}
		public static ResourceKey ToggleButtonGlowMarginKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_Toggle_0004");
			}
		}
		public static ResourceKey ToggleButtonGlowEffectKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Effect), "Blue_Toggle_0005");
			}
		}

		//=============================================================================

		public static ResourceKey ButtonPushStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Push_0020");
			}
		}
		public static ResourceKey ToggleButtonPushStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Push_0021");
			}
		}

		//=============================================================================

		public static ResourceKey ComboBoxDownArrowKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Combo_0001");
			}
		}

		public static ResourceKey ComboReadOnlyButtonStyle
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Combo_0002");
			}
		}

		//=============================================================================

		public static ResourceKey ScrollBarTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0001");
			}
		}
		public static ResourceKey ScrollBarMiddleColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0002");
			}
		}
		public static ResourceKey ScrollBarBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0003");
			}
		}
		public static ResourceKey ScrollButtonTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0004");
			}
		}
		public static ResourceKey ScrollButtonBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0005");
			}
		}
		public static ResourceKey ScrollButtonHighlitedTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0006");
			}
		}
		public static ResourceKey ScrollButtonHighlitedBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0007");
			}
		}
		public static ResourceKey ScrollButtonBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0008");
			}
		}
		public static ResourceKey ScrollButtonGlyphColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0009");
			}
		}
		public static ResourceKey ScrollBarDisabledColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Scroll_0010");
			}
		}
		public static ResourceKey ScrollVerticalBarBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0011");
			}
		}
		public static ResourceKey ScrollHorizontalBarBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0012");
			}
		}
		public static ResourceKey ScrollButtonBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0013");
			}
		}
		public static ResourceKey ScrollButtonGlyphBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0014");
			}
		}
		public static ResourceKey ScrollVerticalButtonBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0015");
			}
		}
		public static ResourceKey ScrollHorizontalButtonBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0016");
			}
		}
		public static ResourceKey ScrollVerticalButtonHilitedBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0017");
			}
		}
		public static ResourceKey ScrollHorizontalButtonHilitedBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0018");
			}
		}
		public static ResourceKey ScrollBarDisabledBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Scroll_0019");
			}
		}
		public static ResourceKey ScrollButtonDownArrowKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Scroll_0020");
			}
		}
		public static ResourceKey ScrollButtonUpArrowKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Scroll_0021");
			}
		}
		public static ResourceKey ScrollButtonRightArrowKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Scroll_0022");
			}
		}
		public static ResourceKey ScrollButtonLeftArrowKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Geometry), "Blue_Scroll_0023");
			}
		}
		public static ResourceKey ScrollBarButtonStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Scroll_0024");
			}
		}
		public static ResourceKey ScrollBarThumbStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Scroll_0025");
			}
		}
		public static ResourceKey ScrollHorizontalBarPageButtonKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Scroll_0026");
			}
		}
		public static ResourceKey ScrollVerticalBarPageButtonKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_Scroll_0027");
			}
		}

		//=============================================================================

		public static ResourceKey ToolBarLightFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0001");
			}
		}
		public static ResourceKey ToolBarMediumFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0002");
			}
		}
		public static ResourceKey ToolBarDarkFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0003");
			}
		}
		public static ResourceKey ToolBarDisabledFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0004");
			}
		}
		public static ResourceKey ToolBarDisabledBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0005");
			}
		}
		public static ResourceKey ToolBarSeparatorFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0006");
			}
		}
		public static ResourceKey ToolBarLightFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0007");
			}
		}
		public static ResourceKey ToolBarMediumFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0008");
			}
		}
		public static ResourceKey ToolBarDarkFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0009");
			}
		}
		public static ResourceKey ToolBarDisabledFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0010");
			}
		}
		public static ResourceKey ToolBarDisabledBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0011");
			}
		}
		public static ResourceKey ToolBarSeparatorFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0012");
			}
		}
		public static ResourceKey ToolBarButtonBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0013");
			}
		}
		public static ResourceKey ToolBarButtonTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0014");
			}
		}
		public static ResourceKey ToolBarButtonBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0015");
			}
		}
		public static ResourceKey ToolBarButtonHoverBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0016");
			}
		}
		public static ResourceKey ToolBarButtonHoverTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0017");
			}
		}
		public static ResourceKey ToolBarButtonHoverBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0018");
			}
		}
		public static ResourceKey ToolBarButtonPressedBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0019");
			}
		}
		public static ResourceKey ToolBarButtonPressedTopColor
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0020");
			}
		}
		public static ResourceKey ToolBarButtonPressedBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0021");
			}
		}
		public static ResourceKey ToolBarButtonCheckedBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0022");
			}
		}
		public static ResourceKey ToolBarButtonCheckedTopColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0023");
			}
		}
		public static ResourceKey ToolBarButtonCheckedBottomColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_ToolBar_0024");
			}
		}
		public static ResourceKey ToolBarButtonBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0025");
			}
		}
		public static ResourceKey ToolBarButtonFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0026");
			}
		}
		public static ResourceKey ToolBarButtonHoverBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0027");
			}
		}
		public static ResourceKey ToolBarButtonHoverBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Style), "Blue_ToolBar_0028");
			}
		}
		public static ResourceKey ToolBarButtonCheckedBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0029");
			}
		}
		public static ResourceKey ToolBarButtonCheckedBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0030");
			}
		}
		public static ResourceKey ToolBarButtonPressedBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0031");
			}
		}
		public static ResourceKey ToolBarButtonPressedBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_ToolBar_0032");
			}
		}
		public static ResourceKey ToolBarButtonGlowMarginKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Thickness), "Blue_ToolBar_0033");
			}
		}
		public static ResourceKey ToolBarButtonGlowEffectKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Effect), "Blue_ToolBar_0034");
			}
		}

		//=============================================================================

		public static ResourceKey TreeArrowFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Tree_0001");
			}
		}
		public static ResourceKey TreeArrowBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Tree_0002");
			}
		}
		public static ResourceKey TreeArrowCheckedFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Tree_0003");
			}
		}
		public static ResourceKey TreeArrowCheckedBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Tree_0004");
			}
		}
		public static ResourceKey TreeArrowHilitedFillColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Tree_0005");
			}
		}
		public static ResourceKey TreeArrowHilitedBorderColorKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_Tree_0006");
			}
		}

		public static ResourceKey TreeArrowFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Tree_0007");
			}
		}
		public static ResourceKey TreeArrowBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Tree_0008");
			}
		}
		public static ResourceKey TreeArrowCheckedFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Tree_0009");
			}
		}
		public static ResourceKey TreeArrowCheckedBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Tree_0010");
			}
		}
		public static ResourceKey TreeArrowHilitedFillBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Tree_0011");
			}
		}
		public static ResourceKey TreeArrowHilitedBorderBrushKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Brush), "Blue_Tree_0012");
			}
		}

		//=============================================================================

		public static ResourceKey ListViewGridStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0001");
			}
		}
		public static ResourceKey ListViewGridTemplateKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0002");
			}
		}
		public static ResourceKey ListViewItemStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0003");
			}
		}
		public static ResourceKey ListViewItemStyleNpNmKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0004");
			}
		}
		public static ResourceKey ListViewItemTemplateKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0005");
			}
		}
		public static ResourceKey ListViewGridItemStyleKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0006");
			}
		}
		public static ResourceKey ListViewGridItemTemplateKey
		{
			get
			{
				return BlueResourceKey.GetResourceKey (typeof (Color), "Blue_List_0007");
			}
		}

		#endregion
	}

	/////////////////////////////////////////////////////////////////////////////

	public class BlueResourceKey : ComponentResourceKey
	{
		internal BlueResourceKey (Type pTypeInTargetAssemnbly, Object pResourceId)
			: base (pTypeInTargetAssemnbly, pResourceId)
		{
		}

		public override System.Reflection.Assembly Assembly
		{
			get
			{
				return GetType ().Assembly;
			}
		}
		public override string ToString ()
		{
			return ResourceId.ToString ();
		}

		static public BlueResourceKey GetResourceKey (Type pTypeInTargetAssemnbly, String pResourceId)
		{
			BlueResourceKey lKey = null;

			lock (mThreadLock)
			{
				try
				{
					if (!mResourceKeys.ContainsKey (pResourceId))
					{
						mResourceKeys[pResourceId] = new BlueResourceKey (pTypeInTargetAssemnbly, pResourceId);
#if DEBUG_NOT
						Debug.Print ("ResourceKey [{0}] [{1}] [{2}]", mResourceKeys[pResourceId].ResourceId, mResourceKeys[pResourceId].TypeInTargetAssembly, mResourceKeys[pResourceId].Assembly);
#endif
					}
					lKey = mResourceKeys[pResourceId];
				}
				catch (Exception pException)
				{
					Debug.Print (pException.Message);
				}
			}
			return lKey;
		}

		static private Dictionary<String, BlueResourceKey> mResourceKeys = new Dictionary<String, BlueResourceKey> ();
		static private Object mThreadLock = new Object ();
	}
}